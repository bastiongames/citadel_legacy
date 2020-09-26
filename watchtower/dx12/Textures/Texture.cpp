#include "Texture.h"

#include "../d3dx12.h"

namespace Citadel::Watchtower::Textures::DX12 {
	Texture::Texture(const ComPtr<ID3D12Device> device, ComPtr<ID3D12GraphicsCommandList> commandList, const TextureDescriptor& descriptor) {
		D3D12_RESOURCE_DESC resourceDesc{};
		resourceDesc.MipLevels = 1;
		resourceDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		resourceDesc.Width = descriptor.width;
		resourceDesc.Height = descriptor.height;
		resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
		resourceDesc.DepthOrArraySize = 1;
		resourceDesc.SampleDesc.Count = 1;
		resourceDesc.SampleDesc.Quality = 0;
		resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		
		device->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE,
			&resourceDesc,
			D3D12_RESOURCE_STATE_COPY_DEST,
			nullptr,
			IID_PPV_ARGS(&texture)
		);

		auto uploadBufferSize = GetRequiredIntermediateSize(texture.Get(), 0, 1);

		
		device->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(uploadBufferSize),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&resourceHeap)
		);

		subresource = {};
		subresource.pData = descriptor.data.get();
		subresource.RowPitch = descriptor.width * 4;
		subresource.SlicePitch = subresource.RowPitch * descriptor.height;
		UpdateSubresources(commandList.Get(), texture.Get(), resourceHeap.Get(), 0, 0, 1, &subresource);

		commandList->ResourceBarrier(
			1,
			&CD3DX12_RESOURCE_BARRIER::Transition(
				texture.Get(),
				D3D12_RESOURCE_STATE_COPY_DEST,
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE
			)
		);
	}
}