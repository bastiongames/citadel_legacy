#include "Shader.h"
#include "keep/base64.h"

#include <vulkan/vulkan.h>

using Citadel::Watchtower::Shader;
using Citadel::Watchtower::Vlkn::ShaderData;

using Citadel::Keep::Encoding::base64_decode;
using Citadel::Keep::MakeSPtr;

handle
Shader::Compile(std::string text, std::string entrypoint, std::string target) {
	auto shaderData = MakeSPtr<ShaderData>();

	std::vector<u8> data = base64_decode(text);

	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = data.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(data.data());

	vkCreateShaderModule()

	return nullptr;
}