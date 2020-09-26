#include "ShaderProgram.h"

#include "Shader.h"

namespace Citadel::Watchtower::Shaders::DX12{
	ShaderProgram::ShaderProgram(const ComPtr<ID3D12Device> device, const ShaderProgramDescriptor& descriptor) {
		vs = descriptor.vertexShader;
		ps = descriptor.pixelShader;
	}

	void*
	ShaderProgram::GetBytecode(ShaderType type) {
		if (type == ShaderType::Vertex) return vs->GetBytecode();
		if (type == ShaderType::Pixel) return ps->GetBytecode();

		throw "No suitable shader found.";
	}

	D3D12_INPUT_LAYOUT_DESC 
	ShaderProgram::GetLayoutDescriptor() {
		auto shader = Keep::CastSPtr<Shaders::Shader, Shader>(vs);
		return shader->GetLayoutDescriptor();
	}

}