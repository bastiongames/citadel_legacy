#include "Device.h"
#include "d3dx12.h"

#include "Swapchain.h"
#include "CommandBuffer.h"
#include "CommandQueue.h"
#include "RenderPass.h"
#include "Buffers/Buffer.h"
#include "Shaders/Shader.h"
#include "Shaders/ShaderProgram.h"
#include "Pipelines/PipelineLayout.h"
#include "Pipelines/GraphicsPipeline.h"
#include "Textures/Texture.h"
#include "ResourceHeap.h"

using Citadel::Watchtower::PSwapchain;
using Citadel::Watchtower::PCommandQueue;
using Citadel::Watchtower::PCommandBuffer;
using Citadel::Watchtower::PRenderPass;

using Citadel::Watchtower::DX12::Device;

using Citadel::Keep::u32;
using Citadel::Keep::MakeSPtr;
using Citadel::Keep::SPtr;

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")

Citadel::Watchtower::PDevice
Citadel::Watchtower::Device::CreateDevice(HWND hwnd) {
	return MakeSPtr<DX12::Device>(hwnd);
}

Device::Device(HWND hwnd) {
	u32 factoryFlags = DXGI_CREATE_FACTORY_DEBUG;
	ComPtr<ID3D12Debug> controller;
	D3D12GetDebugInterface(IID_PPV_ARGS(&controller));
	controller->EnableDebugLayer();
	CreateDXGIFactory2(factoryFlags, IID_PPV_ARGS(&factory));

#define FEATURE_LEVEL D3D_FEATURE_LEVEL_12_0

	ComPtr<IDXGIAdapter1> adapter;
	DXGI_ADAPTER_DESC1 desc;
	for (u32 adapterIndex = 0; DXGI_ERROR_NOT_FOUND != factory->EnumAdapters1(adapterIndex, &adapter); ++adapterIndex) {
		adapter->GetDesc1(&desc);
		if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) continue;
		if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), FEATURE_LEVEL, _uuidof(ID3D12Device), nullptr))) break;
	}
	D3D12CreateDevice(adapter.Get(), FEATURE_LEVEL, IID_PPV_ARGS(&device));

	D3D12_COMMAND_QUEUE_DESC queueDesc{};
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&commandQueue));

	device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&allocator));
	device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, allocator.Get(), nullptr, IID_PPV_ARGS(&commandList));
	commandList->Close();

	device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
	fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	fenceValue = 0;

	this->hwnd = hwnd;
}

PSwapchain
Device::CreateSwapchain(const SwapchainDescriptor& descriptor){
	DX12::SwapchainCreateInfo createInfo;
	{
		createInfo.descriptor = descriptor;
		createInfo.device = device;
		createInfo.factory = factory;
		createInfo.commandQueue = commandQueue;
		createInfo.hwnd = hwnd;
	};
	return MakeSPtr<DX12::Swapchain>(createInfo);
}

PCommandQueue
Device::CreateCommandQueue(const CommandQueueDescriptor& descriptor) {
	return MakeSPtr<DX12::CommandQueue>(device, commandQueue);
}

PCommandBuffer
Device::CreateCommandBuffer(const CommandBufferDescriptor& descriptor) {
	return MakeSPtr<DX12::CommandBuffer>(device);
}

PRenderPass
Device::CreateRenderPass(const PRenderTarget renderTarget) {
	return MakeSPtr<DX12::RenderPass>(renderTarget);
}

Citadel::Watchtower::Buffers::PBuffer
Device::CreateBuffer(const Citadel::Watchtower::Buffers::BufferDescriptor& descriptor, void* data) {
	return Keep::MakeSPtr<Buffers::DX12::Buffer>(device, descriptor, data);
}

Citadel::Watchtower::Shaders::PShader
Device::CreateShader(const Citadel::Watchtower::Shaders::ShaderDescriptor& descriptor) {
	return Keep::MakeSPtr<Shaders::DX12::Shader>(device, descriptor);
}

Citadel::Watchtower::Shaders::PShaderProgram
Device::CreateShaderProgram(const Shaders::ShaderProgramDescriptor& descriptor) {
	return Keep::MakeSPtr<Shaders::DX12::ShaderProgram>(device, descriptor);
}

Citadel::Watchtower::Pipelines::PPipelineLayout
Device::CreatePipelineLayout(const Pipelines::PipelineLayoutDescriptor& descriptor) {
	return Keep::MakeSPtr<Pipelines::DX12::PipelineLayout>(device, descriptor);
}

Citadel::Watchtower::Pipelines::PGraphicsPipeline
Device::CreateGraphicsPipeline(const Pipelines::GraphicsPipelineDescriptor& descriptor) {
	return Keep::MakeSPtr<Pipelines::DX12::GraphicsPipeline>(device, descriptor);
}

Citadel::Watchtower::Textures::PTexture
Device::CreateTexture(const Textures::TextureDescriptor& descriptor) {
	allocator->Reset();
	commandList->Reset(allocator.Get(), nullptr);
	
	auto texture = MakeSPtr<Textures::DX12::Texture>(device, commandList, descriptor);
	
	commandList->Close();
	ID3D12CommandList* lists[] = { commandList.Get() };
	commandQueue->ExecuteCommandLists(1, lists);

	commandQueue->Signal(fence.Get(), fenceValue);
	auto val = fence->GetCompletedValue();
	fence->SetEventOnCompletion(fenceValue, fenceEvent);
	auto res = WaitForSingleObjectEx(fenceEvent, INFINITE, FALSE);
	fenceValue++;

	return texture;
}

Citadel::Watchtower::PResourceHeap
Device::CreateResourceHeap(const ResourceHeapDescriptor& descriptor) {
	return MakeSPtr<ResourceHeap>(device, descriptor);
}