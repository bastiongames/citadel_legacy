#ifndef __VLKN_DEVICEDATA_H__
#define __VLKN_DEVICEDATA_H__

#include <vulkan/vulkan.h>
#include <vector>

namespace Citadel::Watchtower::Vlkn {
	struct DeviceData {
		VkInstance instance;
		VkPhysicalDevice physicalDevice;
		VkDevice device;
		VkSurfaceKHR surface;
		VkQueue graphicsQueue;
		VkQueue presentQueue;

		VkCommandPool commandPool;

		// Swap Chain
		VkSwapchainKHR swapchain;
		std::vector<VkImage> swapchainImages;
		VkFormat swapchainImageFormat;
		VkExtent2D swapchainExtent;
		std::vector<VkImageView> swapchainImageViews;

		// Frame Sync
		u32 frameIndex;
		std::vector<VkSemaphore> imageAvailableSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;

		// additional layers
		VkDebugUtilsMessengerEXT debugMessenger;
		VkDebugReportCallbackEXT reportCallback;
	};
}

#endif