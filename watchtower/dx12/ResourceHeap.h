#ifndef __DX12__RESOURCEHEAP_H__
#define __DX12__RESOURCEHEAP_H__

#include "watchtower/ResourceHeap.h"
#include "Command.h"

#include <d3d12.h>
#include <wrl/client.h>
using Microsoft::WRL::ComPtr;
#include <vector>

namespace Citadel::Watchtower::DX12 {
	class ResourceHeap : public Watchtower::ResourceHeap, public Command {
	public:
		ResourceHeap(const ComPtr<ID3D12Device> device, const ResourceHeapDescriptor& descriptor);
		void BindTexture(const Textures::PTexture texture, Keep::u32 slot);
		void BindBuffer(const Buffers::PBuffer buffer, Keep::u32 slot);
		virtual void Record(const ComPtr<ID3D12GraphicsCommandList> commandList);
	private:
		ComPtr<ID3D12DescriptorHeap> resourceHeap;
		ComPtr<ID3D12DescriptorHeap> samplerHeap;
		ComPtr<ID3D12Device> device;

		//D3D12_CPU_DESCRIPTOR_HANDLE cpuDescriptorHandle;
		//Keep::u32 descriptorHandleStride;
		
	};
}

#endif