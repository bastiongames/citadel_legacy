#include "watchtower/dx12/Device.h"

#include "watchtower/dx12/Pipeline.h"
#include "watchtower/dx12/VertexBuffer.h"
#include "watchtower/dx12/Shader.h"

#include "keep/Types.h"

using Citadel::Watchtower::DX12::DeviceData;
using Citadel::Watchtower::DX12::FrameData;
using Citadel::Watchtower::DX12::PipelineData;
using Citadel::Watchtower::DX12::VertexBufferData;
using Citadel::Watchtower::DX12::ShaderData;

using Citadel::Watchtower::Pipeline;
using Citadel::Watchtower::VertexBuffer;
using Citadel::Watchtower::VertexShader;
using Citadel::Watchtower::PixelShader;

using Citadel::Keep::SPtr;
using Citadel::Keep::MakeSPtr;
using Citadel::Keep::SPtrFromHandle;
using Citadel::Keep::HandleFromSPtr;

// Library includes
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

// Device specific helper functions

/*
	Returns the flags to pass in to the factory based on if the debug layer was enabled
*/
UINT DebugDevice(UINT flags) {

	ComPtr<ID3D12Debug> controller;
	D3D12GetDebugInterface(IID_PPV_ARGS(&controller));
	controller->EnableDebugLayer();
	flags |= DXGI_CREATE_FACTORY_DEBUG;

	return flags;
}

void CreateFactory(SPtr<DeviceData> data, UINT flags) {
	CreateDXGIFactory2(flags, IID_PPV_ARGS(&data->factory));
}

void CreateDevice(SPtr<DeviceData> data) {
#define FEATURE_LEVEL D3D_FEATURE_LEVEL_12_0

	ComPtr<IDXGIAdapter1> adapter;
	for (UINT adapterIndex = 0; DXGI_ERROR_NOT_FOUND != data->factory->EnumAdapters1(adapterIndex, &adapter); ++adapterIndex) {
		DXGI_ADAPTER_DESC1 desc;
		adapter->GetDesc1(&desc);

		// Ignore the software adapter
		if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) continue;

		if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), FEATURE_LEVEL, _uuidof(ID3D12Device), nullptr))) break;
	}

	D3D12CreateDevice(adapter.Get(), FEATURE_LEVEL, IID_PPV_ARGS(&data->device));
}

void CreateCommandQueue(SPtr<DeviceData> data) {
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	data->device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&data->commandQueue));

	CD3DX12_ROOT_SIGNATURE_DESC signatureDesc = {};
	signatureDesc.Init(0, nullptr, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
	ComPtr<ID3DBlob> signature;
	ComPtr<ID3DBlob> errors;
	D3D12SerializeRootSignature(&signatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &errors);
	data->device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&data->signature));
}

void CreateCommandList(SPtr<DeviceData> data) {
	auto frame = data->frames[data->frameIndex];
	data->device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, frame.allocator.Get(), nullptr, IID_PPV_ARGS(&data->commandList));
	data->commandList->Close();
}


void CreateSwapChain(SPtr<DeviceData> data, HWND hwnd, int width, int height) {
	DXGI_SWAP_CHAIN_DESC1 desc = {};
	desc.BufferCount = FRAME_COUNT;
	desc.Width = width;
	desc.Height = height;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	desc.SampleDesc.Count = 1;

	HRESULT hr;
	ComPtr<IDXGISwapChain1> swapChain;
	hr = data->factory->CreateSwapChainForHwnd(data->commandQueue.Get(), hwnd, &desc, nullptr, nullptr, &swapChain);
	hr = data->factory->MakeWindowAssociation(hwnd, DXGI_MWA_NO_ALT_ENTER);
	swapChain.As(&data->swapChain);
}

void CreateFrames(SPtr<DeviceData> data) {
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.NumDescriptors = FRAME_COUNT;
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	data->device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&data->descriptorHeap));
	data->frameSize = data->device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	data->frameIndex = data->swapChain->GetCurrentBackBufferIndex();

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(data->descriptorHeap->GetCPUDescriptorHandleForHeapStart());
	for (UINT n = 0; n < FRAME_COUNT; ++n) {
		FrameData frameData = {};
		frameData.fenceValue = 1;
		data->swapChain->GetBuffer(n, IID_PPV_ARGS(&frameData.target));
		data->device->CreateRenderTargetView(frameData.target.Get(), nullptr, rtvHandle);
		rtvHandle.Offset(1, data->frameSize);

		data->device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&frameData.allocator));

		data->frames[n] = frameData;
	}
}

handle
Citadel::Watchtower::Device::AcquireDevice(handle platformData) {
	auto deviceData = MakeSPtr<DeviceData>();
	HWND hwnd = *(SPtrFromHandle<HWND>(platformData).get());

	UINT factoryFlags = 0;
	factoryFlags = DebugDevice(factoryFlags);

	CreateFactory(deviceData, factoryFlags);
	CreateDevice(deviceData);
	CreateCommandQueue(deviceData);
	CreateSwapChain(deviceData, hwnd, 1024, 768);
	CreateFrames(deviceData);
	CreateCommandList(deviceData);

	deviceData->device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&deviceData->fence));
	deviceData->fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);

	return HandleFromSPtr<DeviceData>(deviceData);
}

void
Citadel::Watchtower::Device::ReleaseDevice() {
	auto deviceData = SPtrFromHandle<DeviceData>(impl);
	
	if (deviceData != nullptr && deviceData->fenceEvent != nullptr) {
		CloseHandle(deviceData->fenceEvent);
		deviceData->fenceEvent = nullptr;
	}
}

Citadel::Watchtower::Pipeline
Citadel::Watchtower::Device::CreatePipeline(VertexShader& vshader, PixelShader& pshader) {
	auto deviceData = SPtrFromHandle<DeviceData>(impl);
	auto pipelineData = MakeSPtr<PipelineData>();

	auto vshaderData = SPtrFromHandle<ShaderData>(vshader.Get());
	auto pshaderData = SPtrFromHandle<ShaderData>(pshader.Get());

	auto frame = deviceData->frames[deviceData->frameIndex];

	D3D12_INPUT_ELEMENT_DESC inputElementDesc[]{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
	};

	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc = {};
	pipelineDesc.InputLayout = { inputElementDesc, _countof(inputElementDesc) };
	pipelineDesc.pRootSignature = deviceData->signature.Get();
	pipelineDesc.VS = CD3DX12_SHADER_BYTECODE(vshaderData->bytecode.Get());
	pipelineDesc.PS = CD3DX12_SHADER_BYTECODE(pshaderData->bytecode.Get());
	pipelineDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	pipelineDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	pipelineDesc.DepthStencilState.DepthEnable = FALSE;
	pipelineDesc.DepthStencilState.StencilEnable = FALSE;
	pipelineDesc.SampleMask = UINT_MAX;
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	pipelineDesc.NumRenderTargets = 1;
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	pipelineDesc.SampleDesc.Count = 1;

	deviceData->device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineData->pipelineState));

	pipelineData->viewport = CD3DX12_VIEWPORT(0.0f, 0.0f, 1024.0f, 768.0f);
	pipelineData->scissorRect = CD3DX12_RECT(0, 0, 1024, 768);

	return Pipeline(HandleFromSPtr<PipelineData>(pipelineData));
}

Citadel::Watchtower::Device&
Citadel::Watchtower::Device::UsingPipeline(Pipeline& pipeline) {
	auto deviceData = SPtrFromHandle<DeviceData>(impl);
	auto pipelineData = SPtrFromHandle<PipelineData>(pipeline.Get());
	auto frame = deviceData->frames[deviceData->frameIndex];

	frame.allocator->Reset();
	deviceData->commandList->Reset(frame.allocator.Get(), pipelineData->pipelineState.Get());
	deviceData->commandList->SetGraphicsRootSignature(deviceData->signature.Get());
	deviceData->commandList->RSSetViewports(1, &pipelineData->viewport);
	deviceData->commandList->RSSetScissorRects(1, &pipelineData->scissorRect);

	deviceData->commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(frame.target.Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(deviceData->descriptorHeap->GetCPUDescriptorHandleForHeapStart(), deviceData->frameIndex, deviceData->frameSize);

	const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
	deviceData->commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
	deviceData->commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

	return *this;
}

Citadel::Watchtower::VertexBuffer
Citadel::Watchtower::Device::CreateVertexBuffer(void* data, u32 size, u32 stride) {
	auto deviceData = SPtrFromHandle<DeviceData>(impl);

	ComPtr<ID3D12Resource> vbuffer;
	deviceData->device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(size),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vbuffer)
	);

	UINT8* vertexDataBegin;
	CD3DX12_RANGE readRange(0, 0);
	vbuffer->Map(0, &readRange, reinterpret_cast<void**>(&vertexDataBegin));
	memcpy(vertexDataBegin, data, size);
	vbuffer->Unmap(0, nullptr);

	D3D12_VERTEX_BUFFER_VIEW view;
	view.BufferLocation = vbuffer->GetGPUVirtualAddress();
	view.StrideInBytes = stride;
	view.SizeInBytes = size;

	auto vbufferData = MakeSPtr<VertexBufferData>();
	vbufferData->vbuffer = vbuffer;
	vbufferData->view = view;

	return VertexBuffer(HandleFromSPtr<VertexBufferData>(vbufferData));
}

Citadel::Watchtower::Device&
Citadel::Watchtower::Device::DrawTriangleList(VertexBuffer& buffer) {
	auto deviceData = SPtrFromHandle<DeviceData>(impl);
	auto vbufferData = SPtrFromHandle<VertexBufferData>(buffer.Get());

	deviceData->commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceData->commandList->IASetVertexBuffers(0, 1, &vbufferData->view);
	deviceData->commandList->DrawInstanced(3, 1, 0, 0); // need to get this from the buffer.

	return *this;
}

Citadel::Watchtower::RenderTarget
Citadel::Watchtower::Device::CreateRenderTarget() {
	return RenderTarget();
}

Citadel::Watchtower::Device&
Citadel::Watchtower::Device::RenderTo(RenderTarget& target) {
	return *this;
}

void
Citadel::Watchtower::Device::Present() {
	auto deviceData = SPtrFromHandle<DeviceData>(impl);
	auto frame = deviceData->frames[deviceData->frameIndex];

	deviceData->commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(frame.target.Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));
	deviceData->commandList->Close();

	// Execute the command list(s)
	ID3D12CommandList* commandLists[] = { deviceData->commandList.Get() };
	deviceData->commandQueue->ExecuteCommandLists(_countof(commandLists), commandLists);
	
	// swap chain present
	deviceData->swapChain->Present(1, 0);
}

void
Citadel::Watchtower::Device::FinishFrame() {
	auto deviceData = SPtrFromHandle<DeviceData>(impl);
	auto frame = deviceData->frames[deviceData->frameIndex];

	const UINT64 currentFenceValue = frame.fenceValue;
	deviceData->commandQueue->Signal(deviceData->fence.Get(), currentFenceValue);

	deviceData->frameIndex = deviceData->swapChain->GetCurrentBackBufferIndex();

	if (deviceData->fence->GetCompletedValue() < frame.fenceValue) {
		deviceData->fence->SetEventOnCompletion(frame.fenceValue, deviceData->fenceEvent);
		WaitForSingleObjectEx(deviceData->fenceEvent, INFINITE, FALSE);
	}

	frame.fenceValue = currentFenceValue + 1;
}

void
Citadel::Watchtower::Device::Wait() {
	auto deviceData = SPtrFromHandle<DeviceData>(impl);
	auto frame = deviceData->frames[deviceData->frameIndex];

	deviceData->commandQueue->Signal(deviceData->fence.Get(), frame.fenceValue);
	deviceData->fence->SetEventOnCompletion(frame.fenceValue, deviceData->fenceEvent);
	WaitForSingleObjectEx(deviceData->fenceEvent, INFINITE, FALSE);
	frame.fenceValue++;
}