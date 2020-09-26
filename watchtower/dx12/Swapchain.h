#ifndef __DX12__SWAPCHAIN_H__
#define __DX12__SWAPCHAIN_H__

#include "watchtower/Swapchain.h"
#include "watchtower/SwapchainDescriptor.h"
#include "RenderTarget.h"

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl/client.h>
using Microsoft::WRL::ComPtr;


#include <vector>

namespace Citadel::Watchtower::DX12 {

	struct FenceData {
		ComPtr<ID3D12Fence> fence;
		HANDLE event;
		Keep::u64 value;

		void WaitForValue(Keep::u64 v, Keep::u32 timeout);
	};

	struct SwapchainCreateInfo {
		Watchtower::SwapchainDescriptor descriptor;
		ComPtr<ID3D12Device> device;
		ComPtr<IDXGIFactory2> factory;

		ComPtr<ID3D12CommandQueue> commandQueue;
		HWND hwnd;
	};

	class Swapchain : public Watchtower::Swapchain, public RenderTarget {
	public:
		Swapchain(const SwapchainCreateInfo& createInfo);
		virtual void Swap();

	public:
		virtual D3D12_CPU_DESCRIPTOR_HANDLE GetRenderTargetHandle();
		virtual ComPtr<ID3D12Resource> GetBackBuffer();
	private:

		ComPtr<ID3D12CommandQueue> commandQueue;
		ComPtr<IDXGISwapChain3> swapchain;
		ComPtr<ID3D12DescriptorHeap> rtvHeap;
		ComPtr<ID3D12DescriptorHeap> dsvHeap;
		Keep::u32 heapSize;

		std::vector<ComPtr<ID3D12Resource>> colorBuffers;
		
		std::vector<Keep::u64> fenceValues;
		FenceData fence;

		Keep::u32 currentFrame;
	};
}

#endif