#ifndef __DX12__RENDERTARGET_H__
#define __DX12__RENDERTARGET_H__

#include "watchtower/RenderTarget.h"
#include <d3d12.h>
#include <wrl/client.h>
using Microsoft::WRL::ComPtr;

#include <vector>

namespace Citadel::Watchtower::DX12 {
	class RenderTarget : public Watchtower::RenderTarget {
	public:
		virtual D3D12_CPU_DESCRIPTOR_HANDLE GetRenderTargetHandle() = 0;
		virtual ComPtr<ID3D12Resource> GetBackBuffer() = 0;
	};
}

#endif