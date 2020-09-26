#ifndef __DX12__PIPELINELAYOUT_H__
#define __DX12__PIPELINELAYOUT_H__

#include "watchtower/Pipelines/PipelineLayout.h"
#include "watchtower/Pipelines/PipelineLayoutDescriptor.h"
#include "../Command.h"

#include "../d3dx12.h"
#include <d3d12.h>
#include <wrl/client.h>
using Microsoft::WRL::ComPtr;

#include <vector>
#include <stdexcept>

namespace Citadel::Watchtower::Pipelines::DX12 {
	// TODO: See if this can be simplified to not require these two classes...
	class RootParameter {
	public:
		RootParameter(D3D12_ROOT_PARAMETER1* rootParameter) {
			rootParam = rootParameter;
		}

		RootParameter(const RootParameter&) = default;
		RootParameter& operator=(const RootParameter&) = default;

		void InitAsConstants(Keep::u32 shaderRegister, Keep::u32 num32BitValues, D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL) {
			rootParam->ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
			rootParam->Constants.ShaderRegister = shaderRegister;
			rootParam->Constants.RegisterSpace = 0;
			rootParam->Constants.Num32BitValues = num32BitValues;
			rootParam->ShaderVisibility = visibility;
		}
		void InitAsDescriptor(D3D12_ROOT_PARAMETER_TYPE parameterType, Keep::u32 shaderRegister, D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL) {
			rootParam->ParameterType = parameterType;
			rootParam->Descriptor.ShaderRegister = shaderRegister;
			rootParam->Descriptor.RegisterSpace = 0;
			rootParam->ShaderVisibility = visibility;
		}
		void InitAsDescriptorCBV(Keep::u32 shaderRegister, D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL) {
			InitAsDescriptor(D3D12_ROOT_PARAMETER_TYPE_CBV, shaderRegister, visibility);
		}
		void InitAsDescriptorSRV(Keep::u32 shaderRegister, D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL) {
			InitAsDescriptor(D3D12_ROOT_PARAMETER_TYPE_SRV, shaderRegister, visibility);
		}
		void InitAsDescriptorUAV(Keep::u32 shaderRegister, D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL) {
			InitAsDescriptor(D3D12_ROOT_PARAMETER_TYPE_UAV, shaderRegister, visibility);
		}
		void InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE rangeType, Keep::u32 shaderRegister, Keep::u32 numDescriptors, D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL) {
			InitAsDescriptorTable(1, visibility);
			AppendDescriptorTableRange(rangeType, shaderRegister, numDescriptors);
		}
		void InitAsDescriptorTable(Keep::u32 maxNumDescriptorRanges, D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL) {
			ranges.reserve(maxNumDescriptorRanges);

			rootParam->ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
			rootParam->DescriptorTable.NumDescriptorRanges = 0;
			rootParam->DescriptorTable.pDescriptorRanges = ranges.data();
			rootParam->ShaderVisibility = visibility;
		}

		void AppendDescriptorTableRange(D3D12_DESCRIPTOR_RANGE_TYPE rangeType, Keep::u32 baseShaderRegister, Keep::u32 numDescriptors = 1, Keep::u32 registerSpace = 0) {
			ranges.resize(ranges.size() + 1);
			auto& range = ranges.back();

			range.RangeType = rangeType;
			range.NumDescriptors = numDescriptors;
			range.BaseShaderRegister = baseShaderRegister;
			range.RegisterSpace = registerSpace;
			range.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
			range.Flags = D3D12_DESCRIPTOR_RANGE_FLAG_NONE;

			++rootParam->DescriptorTable.NumDescriptorRanges;
		}

		void Clear() {
			(*rootParam) = {};
			ranges.clear();
		}

		inline bool AreRangeTypesCompatible(D3D12_DESCRIPTOR_RANGE_TYPE lhs, D3D12_DESCRIPTOR_RANGE_TYPE rhs) const {
			return (
				(lhs == D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER && rhs == D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER) ||
				(lhs != D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER && rhs != D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER)
				);
		}

		bool IsCompatible(D3D12_DESCRIPTOR_RANGE_TYPE rangeType) const {
			if (ranges.empty()) return true;
			return AreRangeTypesCompatible(ranges.back().RangeType, rangeType);
		}
	private:
		D3D12_ROOT_PARAMETER1* rootParam = nullptr;
		std::vector<D3D12_DESCRIPTOR_RANGE1> ranges;
	};
	class RootSignatureBuilder {
	public:
		void Reset(Keep::u32 maxNumRootParameters, Keep::u32 maxNumStaticSamplers) {
			rootParameters.reserve(maxNumRootParameters);
			rootParams.reserve(maxNumRootParameters);
			staticSamplers.reserve(maxNumStaticSamplers);
		}

		void ResetAndAlloc(Keep::u32 maxNumRootParameters, Keep::u32 maxNumStaticSamplers) {
			Reset(maxNumRootParameters, maxNumStaticSamplers);
			while (maxNumRootParameters-- > 0)
				AppendRootParameter();
		}

		RootParameter* AppendRootParameter() {
			rootParameters.push_back({});
			rootParams.emplace_back(&(rootParameters.back()));
			return &(rootParams.back());
		}

		RootParameter* FindCompatibleRootParameter(D3D12_DESCRIPTOR_RANGE_TYPE rangeType) {
			for (auto it = rootParams.rbegin(); it != rootParams.rend(); ++it) {
				if (it->IsCompatible(rangeType))
					return &(*it);
			}
			return nullptr;
		}

		D3D12_STATIC_SAMPLER_DESC* AppendStaticSampler() {
			D3D12_STATIC_SAMPLER_DESC sampler;
			{
				sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
				sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
				sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
				sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
				sampler.MipLODBias = 0.0f;
				sampler.MaxAnisotropy = 1;
				sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
				sampler.BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE;
				sampler.MinLOD = -FLT_MAX;
				sampler.MaxLOD = FLT_MAX;
				sampler.ShaderRegister = 0;
				sampler.RegisterSpace = 0;
				sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
			}
			staticSamplers.push_back(sampler);
			return &(staticSamplers.back());
		}

		ComPtr<ID3D12RootSignature> Build(ComPtr<ID3D12Device> device, D3D12_ROOT_SIGNATURE_FLAGS flags = D3D12_ROOT_SIGNATURE_FLAG_NONE, ComPtr<ID3DBlob>* serializedBlob = nullptr) {
			CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC signatureDesc{};
			signatureDesc.Init_1_1(
				rootParameters.size(),
				rootParameters.data(),
				staticSamplers.size(),
				staticSamplers.data(),
				flags
			);

			return CreateRootSignature(device, signatureDesc, serializedBlob);
		}

		inline ComPtr<ID3D12RootSignature> CreateRootSignature(ComPtr<ID3D12Device> device, const D3D12_VERSIONED_ROOT_SIGNATURE_DESC& signatureDesc, ComPtr<ID3DBlob>* serializedBlob) {
			ComPtr<ID3D12RootSignature> rootSignature;
			auto signature = SerializeRootSignature(signatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_1);
			auto hr = device->CreateRootSignature(
				0,
				signature->GetBufferPointer(),
				signature->GetBufferSize(),
				IID_PPV_ARGS(rootSignature.ReleaseAndGetAddressOf())
			);

			if (serializedBlob != nullptr)
				*serializedBlob = std::move(signature);
			return rootSignature;
		}

		inline ComPtr<ID3DBlob> SerializeRootSignature(const D3D12_VERSIONED_ROOT_SIGNATURE_DESC signatureDesc, const D3D_ROOT_SIGNATURE_VERSION signatureVersion) {
			ComPtr<ID3DBlob> signature, error;
			auto hr = D3DX12SerializeVersionedRootSignature(&signatureDesc, signatureVersion, signature.ReleaseAndGetAddressOf(), error.ReleaseAndGetAddressOf());
			if (error != nullptr) {
				auto err = (const char*)error->GetBufferPointer();
				throw std::runtime_error(err);
			}
			return signature;
		}

		inline const RootParameter& operator[](std::size_t index) const { return rootParams[index]; }
		inline RootParameter& operator[](std::size_t index) { return rootParams[index]; }

	private:
		std::vector<D3D12_ROOT_PARAMETER1> rootParameters;
		std::vector<RootParameter> rootParams;
		std::vector<D3D12_STATIC_SAMPLER_DESC> staticSamplers;
	};


	class PipelineLayout : public Pipelines::PipelineLayout, public Command {
	public:
		PipelineLayout(ComPtr<ID3D12Device> device, const PipelineLayoutDescriptor& descriptor);

		void Record(const ComPtr<ID3D12GraphicsCommandList> commandList);
	public:
		ID3D12RootSignature* GetRootSignature();

	private:
		D3D12_ROOT_SIGNATURE_FLAGS GetRootSignatureFlags(const PipelineLayoutDescriptor& descriptor);
		void BuildRootParameter(RootSignatureBuilder& rootSignature, D3D12_DESCRIPTOR_RANGE_TYPE rangeType, const PipelineLayoutDescriptor& descriptor, const Flags::ResourceType resourceType, const Flags::BindFlags bindFlags, Keep::u32& numResourceViews);
		
		ComPtr<ID3DBlob> signatureBlob;
		ComPtr<ID3D12RootSignature> rootSignature;
	};
}

#endif
