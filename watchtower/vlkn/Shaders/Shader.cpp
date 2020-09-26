#include "Shader.h"

#include "keep/Encoding/Base64.h"

namespace Citadel::Watchtower::Shaders::VLKN {
	Shader::Shader(const VkDevice& device, const ShaderDescriptor& descriptor) {
		auto code = Citadel::Keep::Encoding::Base64::Decode(descriptor.source);

		VkShaderModuleCreateInfo createInfo;
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.pNext = nullptr;
		createInfo.flags = 0;
		createInfo.codeSize = code.size();
		createInfo.pCode = (u32*)code.data();
		
		vkCreateShaderModule(device, &createInfo, nullptr, &bytecode);

		if (descriptor.type == ShaderType::Vertex) {
			for (const auto& attr : descriptor.vertex.input) {
				VkVertexInputAttributeDescription vertexInputAttribute{};
				vertexInputAttribute.location = attr.location;
				vertexInputAttribute.binding = attr.slot;
				vertexInputAttribute.format = GetInputElementFormat(attr.format);
				vertexInputAttribute.offset = attr.offset;

				inputAttributes.push_back(vertexInputAttribute);
			}

			auto& first = descriptor.vertex.input[0];
			VkVertexInputBindingDescription inputBinding{};
			inputBinding.binding = first.slot;
			inputBinding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
			inputBinding.stride = first.stride;
			inputBindings.push_back(inputBinding);
		}
	}

	void
	Shader::PopulateVertexInputCreateInfo(VkPipelineVertexInputStateCreateInfo& createInfo) {
		createInfo.vertexAttributeDescriptionCount = (Keep::u32)inputAttributes.size();
		createInfo.pVertexAttributeDescriptions = inputAttributes.data();
		createInfo.vertexBindingDescriptionCount = (Keep::u32)inputBindings.size();
		createInfo.pVertexBindingDescriptions = inputBindings.data();
		createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	}

	VkFormat
	Shader::GetInputElementFormat(const BufferFormat& format) {
		switch (format) {
		case BufferFormat::RG32Float: return VkFormat::VK_FORMAT_R32G32_SFLOAT;
		case BufferFormat::RGB32Float: return VkFormat::VK_FORMAT_R32G32B32_SFLOAT;
		case BufferFormat::RGBA32Float: return VkFormat::VK_FORMAT_R32G32B32A32_SFLOAT;
		}
		return VkFormat::VK_FORMAT_UNDEFINED;
	}
}