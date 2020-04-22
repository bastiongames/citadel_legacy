#ifndef __DX12__TEXTURE_H__
#define __DX12__TEXTURE_H__

#include <d3d12.h>

#include <wrl/client.h>
using Microsoft::WRL::ComPtr;

namespace Citadel::Watchtower::DX12 {
	struct TextureData {
		ComPtr<ID3D12Resource> texture;
		ComPtr<ID3D12DescriptorHeap> heap;
	};
}

#endif
