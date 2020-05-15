#ifndef __VLKN_VERTEXBUFFERDATA_H__
#define __VLKN_VERTEXBUFFERDATA_H__

#include <vulkan/vulkan.h>

namespace Citadel::Watchtower::Vlkn {
	struct VertexBufferData {
		VkBuffer buffer;
		VkDeviceMemory bufferMemory;
		VkDevice device;

		~VertexBufferData() {
			vkDestroyBuffer(device, buffer, nullptr);
			vkFreeMemory(device, bufferMemory, nullptr);
		}
	};
}

#endif