#ifndef __DX12__BUFFER_H__
#define __DX12__BUFFER_H__

#include "watchtower/Buffers/Buffer.h"
#include "watchtower/Buffers/BufferDescriptor.h"
#include "../Command.h"

#include <d3d12.h>
#include <wrl/client.h>
using Microsoft::WRL::ComPtr;

namespace Citadel::Watchtower::Buffers::DX12{
	class Buffer : public Watchtower::Buffers::Buffer, public Command {
	public:
		Buffer(ComPtr<ID3D12Device> device, const BufferDescriptor& descriptor, void* data);
		virtual void Update(void* data, Keep::u32 size);
		
	public:

		ComPtr<ID3D12Resource> GetBuffer() { return buffer; }
		Keep::u32 GetBufferSize() { return size; }

		void Record(const ComPtr<ID3D12GraphicsCommandList> commandList);

	private:
		ComPtr<ID3D12Resource> buffer;
		union {
			D3D12_VERTEX_BUFFER_VIEW vertexBuffer;
			D3D12_INDEX_BUFFER_VIEW indexBuffer;
		} views;

		BindFlags type;
		Keep::u32 size;
	};
}

#endif