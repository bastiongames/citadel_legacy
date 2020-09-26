#include "ResourceHeap.h"
#include "Textures/Texture.h"
#include "Buffers/Buffer.h"

#include "d3dx12.h"

namespace Citadel::Watchtower::DX12 {
	ResourceHeap::ResourceHeap(ComPtr<ID3D12Device> device, const ResourceHeapDescriptor& descriptor) {
		this->device = device;

		if (descriptor.resourceCount > 0) {
			D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
			heapDesc.NumDescriptors = descriptor.resourceCount;
			heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
			heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

			auto res = device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&resourceHeap));
		}

		if (descriptor.samplerCount > 0) {
			D3D12_DESCRIPTOR_HEAP_DESC samplerHeapDesc{};
			samplerHeapDesc.NumDescriptors = 1;
			samplerHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
			samplerHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

			auto res = device->CreateDescriptorHeap(&samplerHeapDesc, IID_PPV_ARGS(&samplerHeap));
		}
	}

	void
	ResourceHeap::BindTexture(const Textures::PTexture texture, Keep::u32 slot) {
		auto tex = Keep::CastSPtr<Textures::Texture, Textures::DX12::Texture>(texture);

		auto stride = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		CD3DX12_CPU_DESCRIPTOR_HANDLE handle(resourceHeap->GetCPUDescriptorHandleForHeapStart(), slot, stride);

		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;
		device->CreateShaderResourceView(tex->GetTexture().Get(), &srvDesc, handle);

		D3D12_SAMPLER_DESC samplerDesc{};
		samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 1;
		samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;

		device->CreateSampler(&samplerDesc, samplerHeap->GetCPUDescriptorHandleForHeapStart());
	}

	void
	ResourceHeap::BindBuffer(const Buffers::PBuffer buffer, Keep::u32 slot) {

		auto buff = Keep::CastSPtr<Buffers::Buffer, Buffers::DX12::Buffer>(buffer);
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc{};

		auto stride = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		CD3DX12_CPU_DESCRIPTOR_HANDLE handle(resourceHeap->GetCPUDescriptorHandleForHeapStart(), slot, stride);

		cbvDesc.BufferLocation = buff->GetBuffer()->GetGPUVirtualAddress();
		cbvDesc.SizeInBytes = buff->GetBufferSize();

		device->CreateConstantBufferView(&cbvDesc, handle);
	}

	void
	ResourceHeap::Record(const ComPtr<ID3D12GraphicsCommandList> commandList) {
		std::vector<ID3D12DescriptorHeap*> recordHeaps;
		if (resourceHeap.Get() != nullptr) {
			recordHeaps.push_back(resourceHeap.Get());
		}
		if (samplerHeap.Get() != nullptr) {
			recordHeaps.push_back(samplerHeap.Get());
		}

		commandList->SetDescriptorHeaps(recordHeaps.size(), recordHeaps.data());
		
		auto x = 0;
		for (auto i = 0; i < recordHeaps.size(); ++i) {
			auto numDesc = recordHeaps[i]->GetDesc().NumDescriptors;
			auto stride = device->GetDescriptorHandleIncrementSize(recordHeaps[i]->GetDesc().Type);
			auto handle = recordHeaps[i]->GetGPUDescriptorHandleForHeapStart();
			for (auto j = 0; j < numDesc; ++j) {
				commandList->SetGraphicsRootDescriptorTable(x, handle);
				handle.ptr += stride;
				++x;
			}
		}
		
	}
}