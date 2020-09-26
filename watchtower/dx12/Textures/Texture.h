#ifndef __DX12__TEXTURE_H__
#define __DX12__TEXTURE_H__

#include "watchtower/Textures/Texture.h"
#include "watchtower/Textures/TextureDescriptor.h"

#include <d3d12.h>
#include <wrl/client.h>
using Microsoft::WRL::ComPtr;

namespace Citadel::Watchtower::Textures::DX12 {
	class Texture : public Textures::Texture {
	public:
		Texture(const ComPtr<ID3D12Device> device, const ComPtr<ID3D12GraphicsCommandList> commandList, const TextureDescriptor& desciptor);

	public:
		ComPtr<ID3D12Resource> GetTexture() { return texture; }
	private:
		ComPtr<ID3D12Resource> texture;
		D3D12_RESOURCE_DESC textureDescriptor;
		D3D12_SUBRESOURCE_DATA subresource;
		ComPtr<ID3D12Resource> resourceHeap;
	};
}

#endif