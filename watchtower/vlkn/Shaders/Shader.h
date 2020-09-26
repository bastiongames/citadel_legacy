#ifndef __VLKN__SHADER_H__
#define __VLKN__SHADER_H__

#include "watchtower/Shaders/Shader.h"
#include "watchtower/Shaders/ShaderDescriptor.h"

#include <vulkan/vulkan.h>
#include <vector>

namespace Citadel::Watchtower::Shaders::VLKN {
	class Shader : public Shaders::Shader {
	public:
		Shader(const VkDevice& device, const Shaders::ShaderDescriptor& descriptor);
		virtual void* GetBytecode() {
			return &bytecode;
		}

	public:
		void PopulateVertexInputCreateInfo(VkPipelineVertexInputStateCreateInfo& createInfo);

	private:
		VkFormat GetInputElementFormat(const BufferFormat& format);

	private:
		VkShaderModule bytecode;

		// Vertex Shader specific
		std::vector<VkVertexInputBindingDescription> inputBindings;
		std::vector<VkVertexInputAttributeDescription> inputAttributes;
	};
}

#endif