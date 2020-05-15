#include "../Pipeline.h"

#include <vulkan/vulkan.h>
#include <vector>

namespace Citadel::Watchtower::Vlkn {
	struct PipelineData {
		VkRenderPass renderPass;
		VkPipelineLayout layout;
		VkPipeline graphicsPipeline;
		std::vector<VkFramebuffer> framebuffers;
		VkDescriptorSetLayout descriptorSetLayout;

		VkSampler textureSampler;

		VkDevice device;

		~PipelineData() {
			vkDestroySampler(device, textureSampler, nullptr);
			vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);
			for (auto framebuffer : framebuffers) {
				vkDestroyFramebuffer(device, framebuffer, nullptr);
			}
			vkDestroyPipeline(device, graphicsPipeline, nullptr);
			vkDestroyPipelineLayout(device, layout, nullptr);
			vkDestroyRenderPass(device, renderPass, nullptr);
		}
	};
}