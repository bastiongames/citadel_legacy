#ifndef __DX12__SHADER_H__
#define __DX12__SHADER_H__

#include "watchtower/Shaders/Shader.h"
#include "watchtower/Shaders/ShaderDescriptor.h"
#include "keep/StringContainer.h"

#include <d3d12.h>
#include <wrl/client.h>
using Microsoft::WRL::ComPtr;

#include <vector>

namespace Citadel::Watchtower::Shaders::DX12 {
	class Shader : public Shaders::Shader {
	public:
		Shader(const ComPtr<ID3D12Device> device, const ShaderDescriptor& descriptor);

		virtual void* GetBytecode();

	public:
		D3D12_INPUT_LAYOUT_DESC GetLayoutDescriptor();

	private:
		void BuildLayout(const ShaderDescriptor& descriptor);
		DXGI_FORMAT GetInputElementFormat(const BufferFormat& format);
	private:
		ComPtr<ID3DBlob> bytecode;

		// Vertex Shader Specific
		std::vector<D3D12_INPUT_ELEMENT_DESC> inputElements;
		Keep::StringContainer vertexAttributeNames;
	};
}

#endif