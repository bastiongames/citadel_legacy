#include "watchtower/dx12/Device.h"

#include "watchtower/dx12/Context.h"

#include "watchtower/dx12/Pipeline.h"
#include "watchtower/dx12/VertexBuffer.h"
#include "watchtower/dx12/Texture.h"
#include "watchtower/dx12/Shader.h"
#include "watchtower/dx12/RenderTarget.h"

#include "keep/Types.h"

using Citadel::Watchtower::DX12::DeviceData;
using Citadel::Watchtower::DX12::ContextData;
using Citadel::Watchtower::DX12::PipelineData;
using Citadel::Watchtower::DX12::VertexBufferData;
using Citadel::Watchtower::DX12::ShaderData;
using Citadel::Watchtower::DX12::TextureData;

using Citadel::Watchtower::DX12::RenderTargetFrameData;
using Citadel::Watchtower::DX12::RenderTargetData;

using Citadel::Watchtower::Pipeline;
using Citadel::Watchtower::VertexBuffer;
using Citadel::Watchtower::VertexShader;
using Citadel::Watchtower::PixelShader;
using Citadel::Watchtower::Texture;
using Citadel::Watchtower::RenderTarget;

using Citadel::Keep::SPtr;
using Citadel::Keep::MakeSPtr;
using Citadel::Keep::SPtrFromHandle;
using Citadel::Keep::HandleFromSPtr;

// Library includes
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <d3dcompiler.h>

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
}

void CreateCommandList(SPtr<DeviceData> data) {
	data->device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&data->allocator));
	data->device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, data->allocator.Get(), nullptr, IID_PPV_ARGS(&data->commandList));
}


void CreateSwapChain(SPtr<DeviceData> data, HWND hwnd, int width, int height) {
	DXGI_SWAP_CHAIN_DESC1 desc = {};
	desc.BufferCount = RENDER_TARGET_FRAME_COUNT;
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
	CreateCommandList(deviceData);

	deviceData->device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&deviceData->fence));
	deviceData->fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	deviceData->fenceValues[0] = deviceData->fenceValues[1] = 0;

	deviceData->commandList->Close();

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

Citadel::Watchtower::Context2D
Citadel::Watchtower::Device::CreateContext2D() {
	auto deviceData = SPtrFromHandle<DeviceData>(impl);
	auto contextData = MakeSPtr<ContextData>();

	for (int i = 0; i < 2; ++i) {
		deviceData->device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&contextData->allocators[i]));
	}
	deviceData->device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, contextData->allocators[0].Get(), nullptr, IID_PPV_ARGS(&contextData->commandList));

	contextData->commandList->Close();

	return Context2D(HandleFromSPtr<ContextData>(contextData));
}

Citadel::Watchtower::Context3D
Citadel::Watchtower::Device::CreateContext3D() {
	auto deviceData = SPtrFromHandle<DeviceData>(impl);
	auto contextData = MakeSPtr<ContextData>();

	for (int i = 0; i < 2; ++i) {
		deviceData->device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&contextData->allocators[i]));
	}
	deviceData->device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, contextData->allocators[0].Get(), nullptr, IID_PPV_ARGS(&contextData->commandList));

	contextData->commandList->Close();

	return Context3D(HandleFromSPtr<ContextData>(contextData));
}

VertexShader
Citadel::Watchtower::Device::CreateVertexShader(std::string src, std::string entrypoint, std::string model) {
	auto shaderData = MakeSPtr<ShaderData>();
	ComPtr<ID3DBlob> errors;
	HRESULT hr = D3DCompile(src.c_str(), src.size(), nullptr, nullptr, nullptr, 
		entrypoint.c_str(), model.c_str(), 0, 0, &shaderData->bytecode, &errors);
	return VertexShader(HandleFromSPtr<ShaderData>(shaderData));
}

PixelShader
Citadel::Watchtower::Device::CreatePixelShader(std::string src, std::string entrypoint, std::string model) {
	auto shaderData = MakeSPtr<ShaderData>();
	ComPtr<ID3DBlob> errors;
	HRESULT hr = D3DCompile(src.c_str(), src.size(), nullptr, nullptr, nullptr,
		entrypoint.c_str(), model.c_str(), 0, 0, &shaderData->bytecode, &errors);
	return PixelShader(HandleFromSPtr<ShaderData>(shaderData));
}

Citadel::Watchtower::Device&
Citadel::Watchtower::Device::BeginFrame() {
	auto deviceData = SPtrFromHandle<DeviceData>(impl);
	deviceData->commandList->Reset(deviceData->allocator.Get(), nullptr);
	return *this;
}

Citadel::Watchtower::Pipeline
Citadel::Watchtower::Device::CreatePipeline(VertexShader& vshader, PixelShader& pshader) {
	auto deviceData = SPtrFromHandle<DeviceData>(impl);
	auto pipelineData = MakeSPtr<PipelineData>();

	auto vshaderData = SPtrFromHandle<ShaderData>(vshader.Get());
	auto pshaderData = SPtrFromHandle<ShaderData>(pshader.Get());

	D3D12_INPUT_ELEMENT_DESC inputElementDesc[]{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}

		//{"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		//{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 8, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
	};

	pipelineData->viewport = CD3DX12_VIEWPORT(0.0f, 0.0f, 1024.0f, 768.0f);
	pipelineData->scissorRect = CD3DX12_RECT(0, 0, 1024, 768);

	D3D12_FEATURE_DATA_ROOT_SIGNATURE featureData = {};
	featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;
	HRESULT hr = deviceData->device->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE, &featureData, sizeof(featureData));

	CD3DX12_DESCRIPTOR_RANGE1 ranges[1];
	ranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC);
	CD3DX12_ROOT_PARAMETER1 rootParameters[1];
	rootParameters[0].InitAsDescriptorTable(1, &ranges[0], D3D12_SHADER_VISIBILITY_PIXEL);

	D3D12_STATIC_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	samplerDesc.MipLODBias = 0;
	samplerDesc.MaxAnisotropy = 0;
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	samplerDesc.MinLOD = 0.0f;
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
	samplerDesc.ShaderRegister = 0;
	samplerDesc.RegisterSpace = 0;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC signatureDesc = {};
	signatureDesc.Init_1_1(_countof(rootParameters), rootParameters, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> signature;
	ComPtr<ID3DBlob> errors;
	D3DX12SerializeVersionedRootSignature(&signatureDesc, featureData.HighestVersion, &signature, &errors);
	deviceData->device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&pipelineData->signature));

	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc = {};
	pipelineDesc.InputLayout = { inputElementDesc, _countof(inputElementDesc) };
	pipelineDesc.pRootSignature = pipelineData->signature.Get();
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

	return Pipeline(HandleFromSPtr<PipelineData>(pipelineData));
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

Citadel::Watchtower::Texture
Citadel::Watchtower::Device::CreateTexture(handle data, u32 width, u32 height, u32 pixelSize) {
	auto deviceData = SPtrFromHandle<DeviceData>(impl);
	auto textureData = MakeSPtr<TextureData>();

	deviceData->commandList->Reset(deviceData->allocator.Get(), nullptr);

	D3D12_DESCRIPTOR_HEAP_DESC shaderHeapDesc = {};
	shaderHeapDesc.NumDescriptors = 1;
	shaderHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	shaderHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	deviceData->device->CreateDescriptorHeap(&shaderHeapDesc, IID_PPV_ARGS(&textureData->heap));
	
	ComPtr<ID3D12Resource> resourceHeap;

	D3D12_RESOURCE_DESC resourceDesc = {};
	resourceDesc.MipLevels = 1;
	resourceDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	resourceDesc.Width = width;
	resourceDesc.Height = height;
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.SampleDesc.Quality = 0;
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	deviceData->device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&textureData->texture)
	);

	const UINT64 uploadBufferSize = GetRequiredIntermediateSize(textureData->texture.Get(), 0, 1);
	deviceData->device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(uploadBufferSize),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&resourceHeap)
	);

	D3D12_SUBRESOURCE_DATA resourceData = {};
	resourceData.pData = data.get();
	resourceData.RowPitch = width * pixelSize;
	resourceData.SlicePitch = resourceData.RowPitch * height;
	UpdateSubresources(deviceData->commandList.Get(), textureData->texture.Get(), resourceHeap.Get(), 0, 0, 1, &resourceData);
	deviceData->commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(textureData->texture.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = resourceDesc.Format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	deviceData->device->CreateShaderResourceView(textureData->texture.Get(), &srvDesc, textureData->heap->GetCPUDescriptorHandleForHeapStart());

	deviceData->commandList->Close();
	ID3D12CommandList* commandLists[] = { deviceData->commandList.Get() };
	deviceData->commandQueue->ExecuteCommandLists(_countof(commandLists), commandLists);
	Wait();
	return Texture(textureData);
}

Citadel::Watchtower::RenderTarget
Citadel::Watchtower::Device::CreateRenderTarget() {
	auto deviceData = SPtrFromHandle<DeviceData>(impl);
	auto targetData = MakeSPtr<RenderTargetData>();

	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.NumDescriptors = RENDER_TARGET_FRAME_COUNT;
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	deviceData->device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&targetData->heap));

	targetData->size= deviceData->device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	deviceData->frameIndex = deviceData->swapChain->GetCurrentBackBufferIndex();

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(targetData->heap->GetCPUDescriptorHandleForHeapStart());
	for (UINT n = 0; n < heapDesc.NumDescriptors; ++n) {
		RenderTargetFrameData frameData = {};
		deviceData->swapChain->GetBuffer(n, IID_PPV_ARGS(&frameData.target));
		deviceData->device->CreateRenderTargetView(frameData.target.Get(), nullptr, rtvHandle);
		rtvHandle.Offset(1, targetData->size);

		deviceData->device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&frameData.allocator));

		targetData->frames[n] = frameData;
	}

	return RenderTarget(HandleFromSPtr<RenderTargetData>(targetData));
}

void
Citadel::Watchtower::Device::Present(RenderTarget& target) {
	auto deviceData = SPtrFromHandle<DeviceData>(impl);
	auto targetData = SPtrFromHandle<RenderTargetData>(target.Get());

	auto currentFrame = targetData->frames[deviceData->frameIndex];

	deviceData->commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(currentFrame.target.Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));
	deviceData->commandList->Close();

	// Execute the command list(s)
	ID3D12CommandList* commandLists[] = { deviceData->commandList.Get() };
	deviceData->commandQueue->ExecuteCommandLists(_countof(commandLists), commandLists);
	
	// swap chain present
	deviceData->swapChain->Present(1, 0);
}

void
Citadel::Watchtower::Device::Present(Context& context) {
	auto deviceData = SPtrFromHandle<DeviceData>(impl);
	auto contextData = SPtrFromHandle<ContextData>(context.Get());

	ID3D12CommandList* commandLists[] = { contextData->commandList.Get() };
	deviceData->commandQueue->ExecuteCommandLists(_countof(commandLists), commandLists);

	deviceData->swapChain->Present(0, 0);
}

u32
Citadel::Watchtower::Device::FinishFrame() {
	auto deviceData = SPtrFromHandle<DeviceData>(impl);

	const UINT64 currentFenceValue = deviceData->fenceValues[deviceData->frameIndex];
	deviceData->commandQueue->Signal(deviceData->fence.Get(), currentFenceValue);

	deviceData->frameIndex = deviceData->swapChain->GetCurrentBackBufferIndex();
	
	const UINT64 completedValue = deviceData->fence->GetCompletedValue();
	if (completedValue < deviceData->fenceValues[deviceData->frameIndex]) {
		HRESULT hr = deviceData->fence->SetEventOnCompletion(deviceData->fenceValues[deviceData->frameIndex], deviceData->fenceEvent);
		WaitForSingleObjectEx(deviceData->fenceEvent, INFINITE, FALSE);
	}

	deviceData->fenceValues[deviceData->frameIndex] = currentFenceValue + 1;

	return deviceData->frameIndex;
}

void
Citadel::Watchtower::Device::Wait() {
	auto deviceData = SPtrFromHandle<DeviceData>(impl);
	
	deviceData->commandQueue->Signal(deviceData->fence.Get(), deviceData->fenceValues[deviceData->frameIndex]);
	deviceData->fence->SetEventOnCompletion(deviceData->fenceValues[deviceData->frameIndex], deviceData->fenceEvent);
	WaitForSingleObjectEx(deviceData->fenceEvent, INFINITE, FALSE);
	deviceData->fenceValues[deviceData->frameIndex]++;
}