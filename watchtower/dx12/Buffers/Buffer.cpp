#include "Buffer.h"

#include "../d3dx12.h"

template <typename T>
T GetAlignedSize(T size, T alignment) {
	if (alignment > 1) {
		const T alignmentBitmask = alignment - 1;
		return ((size + alignmentBitmask) & ~alignmentBitmask);
	}
	return size;
}

namespace Citadel::Watchtower::Buffers::DX12 {
	Buffer::Buffer(ComPtr<ID3D12Device> device, const BufferDescriptor& descriptor, void* data) {
		type = descriptor.bindFlags;

		Keep::u32 alignment = 1;
		if ((descriptor.bindFlags & BindFlags::ConstantBuffer) == BindFlags::ConstantBuffer) alignment = 256;
		size = GetAlignedSize<Keep::u64>(descriptor.size, alignment);

		device->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(size),//, D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&buffer)
		);

		views = {};
		if ((descriptor.bindFlags & BindFlags::VertexBuffer) == BindFlags::VertexBuffer) {
			views.vertexBuffer.BufferLocation = buffer->GetGPUVirtualAddress();
			views.vertexBuffer.SizeInBytes = size;
			views.vertexBuffer.StrideInBytes = descriptor.vertexAttributes.front().stride;
		}
		if ((descriptor.bindFlags & BindFlags::IndexBuffer) == BindFlags::IndexBuffer) {
			views.indexBuffer.BufferLocation = buffer->GetGPUVirtualAddress();
			views.indexBuffer.SizeInBytes = size;
			views.indexBuffer.Format = DXGI_FORMAT_R16_UINT;
		}

		Keep::u8* dataBegin;
		CD3DX12_RANGE readRange(0, 0);
		buffer->Map(0, &readRange, reinterpret_cast<void**>(&dataBegin));
		memcpy(dataBegin, data, size);
		buffer->Unmap(0, nullptr);
	}

	void
	Buffer::Update(void* data, Keep::u32 size) {
		Keep::u8* dataBegin;
		CD3DX12_RANGE readRange(0, 0);
		buffer->Map(0, &readRange, reinterpret_cast<void**>(&dataBegin));
		memcpy(dataBegin, data, size);
		buffer->Unmap(0, nullptr);
	}

	void
	Buffer::Record(const ComPtr<ID3D12GraphicsCommandList> commandList) {
		if (type == BindFlags::VertexBuffer) {
			commandList->IASetVertexBuffers(0, 1, &views.vertexBuffer);
			commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		}
		else if(type == BindFlags::IndexBuffer) {
			commandList->IASetIndexBuffer(&views.indexBuffer);
		}
	}
}