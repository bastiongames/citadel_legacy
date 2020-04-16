#ifndef __RENDERTARGET_DX12_H__
#define __RENDERTARGET_DX12_H__

#include <d3d12.h>
#include <wrl/client.h>
using Microsoft::WRL::ComPtr;

#include "keep/Types.h"
using Citadel::Keep::UPtr;

namespace Citadel::Watchtower::DX12 {
	struct RenderTargetData {
		ComPtr<ID3D12DescriptorHeap> heap;
		UINT size;
		ComPtr<ID3D12Resource> renderTargets[2];
	};
}

#endif