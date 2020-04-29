#ifndef __RENDERTARGET_DX12_H__
#define __RENDERTARGET_DX12_H__

#include <d3d12.h>
#include <wrl/client.h>
using Microsoft::WRL::ComPtr;

#include "../RenderTarget.h"

namespace Citadel::Watchtower::DX12 {
	#define RENDER_TARGET_FRAME_COUNT 2

	struct RenderTargetFrameData {
		ComPtr<ID3D12CommandAllocator> allocator;
		ComPtr<ID3D12Resource> target;
	};

	struct RenderTargetData {
		ComPtr<ID3D12DescriptorHeap> heap;
		UINT size;
		RenderTargetFrameData frames[RENDER_TARGET_FRAME_COUNT];
	};
}

#endif