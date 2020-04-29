#ifndef __DX12__DEVICE_H__
#define __DX12__DEVICE_H__

#include <wrl/client.h>
using Microsoft::WRL::ComPtr;

#include <d3d12.h>
#include <dxgi1_6.h>
#include "d3dX12.h"
#include "../Device.h"

namespace Citadel::Watchtower::DX12 {
	struct DeviceData {
		ComPtr<IDXGIFactory2> factory;
		ComPtr<ID3D12Device> device;


		ComPtr<ID3D12CommandQueue> commandQueue;
		ComPtr<IDXGISwapChain3> swapChain;
		
		// This command list/allocator pair is only used internally to pre-create resources
		ComPtr<ID3D12GraphicsCommandList> commandList;
		ComPtr<ID3D12CommandAllocator> allocator;

		UINT frameIndex;
		
		ComPtr<ID3D12Fence> fence;
		UINT64 fenceValues[2];
		HANDLE fenceEvent;
	};
}

#endif