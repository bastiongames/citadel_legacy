#include "Shader.h"

#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

namespace Citadel::Watchtower::Shaders::DX12 {
	Shader::Shader(const ComPtr<ID3D12Device> device, const ShaderDescriptor& descriptor) {
		ComPtr<ID3DBlob> errors;
		auto res = D3DCompile(
			descriptor.source,
			strlen(descriptor.source),
			nullptr,
			nullptr,
			nullptr,
			descriptor.entryPoint,
			descriptor.profile,
			0,
			0,
			&bytecode,
			&errors
		);

		if (errors != nullptr) {
			char* errorMsg = (char*)errors->GetBufferPointer();
			int a = 0; ++a;
		}

		if (descriptor.type == ShaderType::Vertex) {
			BuildLayout(descriptor);
		}
	}

	void*
	Shader::GetBytecode() {
		return bytecode.Get();
	}

	D3D12_INPUT_LAYOUT_DESC
	Shader::GetLayoutDescriptor() {
		return { inputElements.data(), (Keep::u32)inputElements.size() };
	}

	void
	Shader::BuildLayout(const ShaderDescriptor& descriptor) {
		vertexAttributeNames.Clear();
		for (const auto& attr : descriptor.vertex.input)
			vertexAttributeNames.Reserve(attr.name.size());

		for (auto attribute : descriptor.vertex.input) {
			D3D12_INPUT_ELEMENT_DESC inputElement{};
			inputElement.SemanticName = vertexAttributeNames.CopyString(attribute.name);
			inputElement.SemanticIndex = attribute.semanticIndex;
			inputElement.InputSlot = attribute.slot;
			inputElement.AlignedByteOffset = attribute.offset;
			inputElement.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
			inputElement.InstanceDataStepRate = 0;
			inputElement.Format = GetInputElementFormat(attribute.format);

			inputElements.push_back(inputElement);
		}
	}

	DXGI_FORMAT
	Shader::GetInputElementFormat(const BufferFormat& format) {
		switch (format) {
		case BufferFormat::RGBA32Float: return DXGI_FORMAT_R32G32B32A32_FLOAT;
		case BufferFormat::RGB32Float: return DXGI_FORMAT_R32G32B32_FLOAT;
		case BufferFormat::RG32Float: return DXGI_FORMAT_R32G32_FLOAT;
		}
		return DXGI_FORMAT_UNKNOWN;
	}
}