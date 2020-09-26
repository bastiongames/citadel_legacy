#ifndef __VLKN__SHADERPROGRAM_H__
#define __VLKN__SHADERPROGRAM_H__

#include "watchtower/Shaders/ShaderProgram.h"
#include "watchtower/Shaders/ShaderProgramDescriptor.h"

#include "vulkan/vulkan.h"

namespace Citadel::Watchtower::Shaders::VLKN {
	class ShaderProgram : public Shaders::ShaderProgram {
	public:
		ShaderProgram(const ShaderProgramDescriptor& descriptor);
		virtual void* GetBytecode(Shaders::ShaderType type);
	
	public:
		void PopulateVertexInputCreateInfo(VkPipelineVertexInputStateCreateInfo& createInfo);

	private:
		PShader vs;
		PShader ps;
	};
}

#endif