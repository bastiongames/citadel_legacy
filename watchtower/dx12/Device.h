#ifndef __DX12__DEVICE_H__
#define __DX12__DEVICE_H__

#include <wrl/client.h>
using Microsoft::WRL::ComPtr;

#include <d3d12.h>
#include <dxgi1_6.h>
#include "d3dX12.h"
#include "../Device.h"

namespace Citadel::Watchtower::DX12 {
/*
#define FRAME_COUNT 2

	struct FrameData {
		ComPtr<ID3D12CommandAllocator> allocator;
		ComPtr<ID3D12Resource> target;
		UINT64 fenceValue;
	};
*/
	struct DeviceData {
		ComPtr<IDXGIFactory2> factory;
		ComPtr<ID3D12Device> device;
		ComPtr<ID3D12CommandQueue> commandQueue;
		ComPtr<ID3D12GraphicsCommandList> commandList;
		ComPtr<IDXGISwapChain3> swapChain;

		//ComPtr<ID3D12RootSignature> signature;
		//ComPtr<ID3D12DescriptorHeap> descriptorHeap;
		//ComPtr<ID3D12DescriptorHeap> shaderHeap;
		ComPtr<ID3D12CommandAllocator> allocator;

		CD3DX12_VIEWPORT viewport;
		CD3DX12_RECT scissorRect;

		UINT frameIndex;
		//UINT frameSize;
		//FrameData frames[FRAME_COUNT];
		
		ComPtr<ID3D12Fence> fence;
		UINT64 fenceValues[2];
		HANDLE fenceEvent;
	};
}

#endif