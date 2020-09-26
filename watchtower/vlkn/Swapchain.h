#ifndef __DX12__SWAPCHAIN_H__
#define __DX12__SWAPCHAIN_H__

#include "watchtower/Swapchain.h"
#include "watchtower/SwapchainDescriptor.h"

#include "RenderTarget.h"

#include <windows.h>
#include <vulkan/vulkan.h>

#include <vector>

namespace Citadel::Watchtower::VLKN {
	struct SwapchainCreateInfo {
		Watchtower::SwapchainDescriptor descriptor;

		VkInstance instance;
		VkPhysicalDevice physicalDevice;
		VkDevice device;
		VkSurfaceKHR surface;

		VkQueue presentQueue;
		Keep::u32 presentFamily;
	};

	class Swapchain : public Watchtower::Swapchain, public VLKN::RenderTarget {
	public:
		Swapchain(const SwapchainCreateInfo& createInfo);
		virtual const std::vector<VkImageView> GetViews() override;

		virtual void Swap() override;


	private:
		void Record(Keep::u32 queueFamily);
		

	private:
		VkSwapchainKHR swapchain;
		std::vector<VkImage> images;
		std::vector<VkImageView> imageViews;
		std::vector<VkFramebuffer> framebuffers;
		VkSemaphore imageAvailable;
		VkSemaphore renderFinished;

		VkDevice device;
		VkQueue queue;
		std::vector<VkCommandBuffer> commandBuffers;
		VkCommandPool commandPool;
		VkRenderPass renderPass;

		Keep::u32 imageIndex;
	};

}

#endif