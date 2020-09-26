#include "Swapchain.h"

#include "d3dx12.h"

#define MAX(a,b) (a>b?a:b)

namespace Citadel::Watchtower::DX12 {

	void
	FenceData::WaitForValue(Keep::u64 v, Keep::u32 timeout) {
		value = MAX(value, v) + 1;
		const Keep::u64 signaledValue = fence->GetCompletedValue();
		if (signaledValue < value) {
			HRESULT hr = fence->SetEventOnCompletion(v, event);
			auto res = WaitForSingleObjectEx(event, INFINITE, false);

			int a = 0;
		}
	}


	Swapchain::Swapchain(const SwapchainCreateInfo& createInfo) {
		currentFrame = 0;
		heapSize = createInfo.device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		{
			D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
			heapDesc.NumDescriptors = createInfo.descriptor.frameCount;
			heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
			heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			createInfo.device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&rtvHeap));
		}

		{
			DXGI_SWAP_CHAIN_DESC1 swapchainDesc{};
			swapchainDesc.BufferCount = createInfo.descriptor.frameCount;
			swapchainDesc.Width = createInfo.descriptor.width;
			swapchainDesc.Height = createInfo.descriptor.height;
			swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;// DXGI_FORMAT_R8G8B8A8_UNORM;
			swapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
			swapchainDesc.SampleDesc.Count = 1;

			ComPtr<IDXGISwapChain1> tempSwapchain;
			createInfo.factory->CreateSwapChainForHwnd(createInfo.commandQueue.Get(), createInfo.hwnd,
				&swapchainDesc, nullptr, nullptr, &tempSwapchain);
			createInfo.factory->MakeWindowAssociation(createInfo.hwnd, DXGI_MWA_NO_ALT_ENTER);
			tempSwapchain.As(&swapchain);
		}

		{
			colorBuffers.resize(createInfo.descriptor.frameCount);
			CD3DX12_CPU_DESCRIPTOR_HANDLE descHandle(rtvHeap->GetCPUDescriptorHandleForHeapStart());
			for (Keep::u32 i = 0; i < createInfo.descriptor.frameCount; ++i) {
				swapchain->GetBuffer(i, IID_PPV_ARGS(&colorBuffers[i]));
				createInfo.device->CreateRenderTargetView(colorBuffers[i].Get(), nullptr, descHandle);
				descHandle.Offset(1, heapSize);
			}
		}

		{
			createInfo.device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence.fence));
			fence.event = CreateEvent(nullptr, false, false, nullptr);
			fence.value = 0;
			fenceValues.resize(createInfo.descriptor.frameCount);
		}

		commandQueue = createInfo.commandQueue;
	}

	void
	Swapchain::Swap() {
		swapchain->Present(0, 0);

		commandQueue->Signal(fence.fence.Get(), fenceValues[currentFrame]);
		currentFrame = swapchain->GetCurrentBackBufferIndex();
		fence.WaitForValue(fenceValues[currentFrame], ~0);
		fenceValues[currentFrame] = fence.value;
	}

	D3D12_CPU_DESCRIPTOR_HANDLE
	Swapchain::GetRenderTargetHandle() {
		auto handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(
			rtvHeap->GetCPUDescriptorHandleForHeapStart(),
			currentFrame,
			heapSize
		);

		return handle;
	}

	ComPtr<ID3D12Resource>
	Swapchain::GetBackBuffer() {
		return colorBuffers[currentFrame];
	}
}