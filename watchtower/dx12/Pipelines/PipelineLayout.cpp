#include "PipelineLayout.h"
#include "../d3dx12.h"

using Citadel::Keep::u32;

namespace Citadel::Watchtower::Pipelines::DX12 {
	
	PipelineLayout::PipelineLayout(ComPtr<ID3D12Device> device, const PipelineLayoutDescriptor& descriptor) {
		D3D12_FEATURE_DATA_ROOT_SIGNATURE featureData{};
		{
			featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;
			if (FAILED(device->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE, &featureData, sizeof(featureData)))) {
				featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_0;
			}
		}
		
		{
			std::vector<CD3DX12_DESCRIPTOR_RANGE1> cbvRanges;
			CD3DX12_DESCRIPTOR_RANGE1 cbvRange;
			cbvRange.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC);
			cbvRanges.push_back(cbvRange);

			std::vector<CD3DX12_DESCRIPTOR_RANGE1> samplerRanges;
			CD3DX12_DESCRIPTOR_RANGE1 samplerRange;
			samplerRange.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER, 1, 0);
			samplerRanges.push_back(samplerRange);

			std::vector<CD3DX12_DESCRIPTOR_RANGE1> srvRanges;
			CD3DX12_DESCRIPTOR_RANGE1 srvRange;
			srvRange.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC);
			srvRanges.push_back(srvRange);

			std::vector<CD3DX12_ROOT_PARAMETER1> rootParameters;
			for (const auto& binding : descriptor.bindings) {
				if (binding.type == Flags::ResourceType::Buffer) {
					CD3DX12_ROOT_PARAMETER1 vertexRootParameter;
					vertexRootParameter.InitAsDescriptorTable(cbvRanges.size(), cbvRanges.data(), D3D12_SHADER_VISIBILITY_VERTEX);
					rootParameters.push_back(vertexRootParameter);
				}

				if (binding.type == Flags::ResourceType::Texture) {
					CD3DX12_ROOT_PARAMETER1 pixelRootParameter;
					pixelRootParameter.InitAsDescriptorTable(srvRanges.size(), srvRanges.data(), D3D12_SHADER_VISIBILITY_PIXEL);
					rootParameters.push_back(pixelRootParameter);
				}

				if (binding.type == Flags::ResourceType::Sampler) {
					CD3DX12_ROOT_PARAMETER1 samplerRootParameter;
					samplerRootParameter.InitAsDescriptorTable(samplerRanges.size(), samplerRanges.data(), D3D12_SHADER_VISIBILITY_PIXEL);
					rootParameters.push_back(samplerRootParameter);
				}
			}
			CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc{};
			rootSignatureDesc.Init_1_1(rootParameters.size(), rootParameters.data(), 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

			ComPtr<ID3DBlob> signature;
			ComPtr<ID3DBlob> errors;
			D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, featureData.HighestVersion, &signature, &errors);
			if (errors != nullptr) {
				auto msg = (const char*)errors->GetBufferPointer();
				throw std::runtime_error(msg);
			}
			device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
		}
	}

	void
	PipelineLayout::Record(const ComPtr<ID3D12GraphicsCommandList> commandList) {
		commandList->SetGraphicsRootSignature(rootSignature.Get());
	}

	ID3D12RootSignature*
	PipelineLayout::GetRootSignature() {
		return rootSignature.Get();
	}

	D3D12_ROOT_SIGNATURE_FLAGS
	PipelineLayout::GetRootSignatureFlags(const PipelineLayoutDescriptor& descriptor) {
		D3D12_ROOT_SIGNATURE_FLAGS signatureFlags = D3D12_ROOT_SIGNATURE_FLAG_NONE;

		signatureFlags |= D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
		signatureFlags |= D3D12_ROOT_SIGNATURE_FLAG_ALLOW_STREAM_OUTPUT;

		Flags::StageFlags stageFlags = Flags::StageFlags::Undefined;
		for (const auto& binding : descriptor.bindings) {
			stageFlags = stageFlags | binding.stageFlags;
		}

		if ((stageFlags & Flags::StageFlags::VertexStage) == Flags::StageFlags::Undefined) signatureFlags |= D3D12_ROOT_SIGNATURE_FLAG_DENY_VERTEX_SHADER_ROOT_ACCESS;
		if ((stageFlags & Flags::StageFlags::HullStage) == Flags::StageFlags::Undefined) signatureFlags |= D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS;
		if ((stageFlags & Flags::StageFlags::DomainStage) == Flags::StageFlags::Undefined) signatureFlags |= D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS;
		if ((stageFlags & Flags::StageFlags::GeometryStage) == Flags::StageFlags::Undefined) signatureFlags |= D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;
		if ((stageFlags & Flags::StageFlags::PixelStage) == Flags::StageFlags::Undefined) signatureFlags |= D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS;

		return signatureFlags;
	}

	void
	PipelineLayout::BuildRootParameter(RootSignatureBuilder& rootSignature, D3D12_DESCRIPTOR_RANGE_TYPE rangeType, const PipelineLayoutDescriptor& descriptor, const Flags::ResourceType resourceType, const Flags::BindFlags bindFlags, u32& numResourceViews) {
		for (const auto& binding : descriptor.bindings) {
			if (binding.type == resourceType && (bindFlags == Flags::BindFlags::None || (binding.bindFlags & bindFlags) != Flags::BindFlags::None)) {
				if (auto rootParam = rootSignature.FindCompatibleRootParameter(rangeType)) {
					rootParam->AppendDescriptorTableRange(rangeType, binding.slot, binding.arraySize);
				}
				else {
					rootParam = rootSignature.AppendRootParameter();
					rootParam->InitAsDescriptorTable((u32)descriptor.bindings.size());
					rootParam->AppendDescriptorTableRange(rangeType, binding.slot, binding.arraySize);
				}
				++numResourceViews;
			}

		}
	}
}