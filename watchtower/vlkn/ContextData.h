#ifndef __VLKN_CONTACT_H__
#define __VLKN_CONTACT_H__

#include <vulkan/vulkan.h>
#include <vector>

namespace Citadel::Watchtower::Vlkn {
	struct ContextData {
		std::vector<VkCommandBuffer> commandBuffers;
		std::vector<VkFence> consumedFences;

		u32 currentFrame;
		VkDevice device;

		~ContextData() {
			for (auto fence : consumedFences) {
				vkDestroyFence(device, fence, nullptr);
			}
		}
	};
}

#endif