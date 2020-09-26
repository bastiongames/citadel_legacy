#include "ShaderProgram.h"
#include "Shader.h"

#include <vulkan/vulkan.h>

namespace Citadel::Watchtower::Shaders::VLKN {
	ShaderProgram::ShaderProgram(const ShaderProgramDescriptor& descriptor) {
		vs = descriptor.vertexShader;
		ps = descriptor.pixelShader;
	}

	void*
	ShaderProgram::GetBytecode(ShaderType type) {
		if (type == ShaderType::Vertex) return vs->GetBytecode();
		if (type == ShaderType::Pixel) return ps->GetBytecode();

		throw "No valid shader found";
	}

	void
	ShaderProgram::PopulateVertexInputCreateInfo(VkPipelineVertexInputStateCreateInfo& createInfo) {
		auto vertexShader = Keep::CastSPtr<Shaders::Shader, Shader>(vs);
		vertexShader->PopulateVertexInputCreateInfo(createInfo);
	}
}