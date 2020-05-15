#ifndef __VLKN_SHADER_H__
#define __VLKN_SHADER_H__

#include "../Shader.h"

#include <vulkan/vulkan.h>
#include <vector>

namespace Citadel::Watchtower::Vlkn {
	struct ShaderData {
		VkShaderModule module;
		VkDevice device; //This reference is needed to destroy the shader module.

		~ShaderData() {
			vkDestroyShaderModule(device, module, nullptr);
		}
	};

	struct VertexShaderData : ShaderData {
		VkVertexInputBindingDescription bindings;
		std::vector<VkVertexInputAttributeDescription> attributes;
	};
}

#endif