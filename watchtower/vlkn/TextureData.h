#ifndef __VLKN_TEXTUREDATA_H_
#define __VLKN_TEXTUREDATA_H_

#include <vulkan/vulkan.h>
namespace Citadel::Watchtower::Vlkn {
	struct TextureData {
		VkImage image;
		VkDeviceMemory memory;
		VkImageView view;

		VkDevice device;

		~TextureData() {
			vkDestroyImageView(device, view, nullptr);
			vkDestroyImage(device, image, nullptr);
			vkFreeMemory(device, memory, nullptr);
		}
	};
}

#endif