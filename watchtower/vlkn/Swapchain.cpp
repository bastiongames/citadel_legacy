#include "Swapchain.h"

#include <Windows.h>
#include <vulkan/vulkan_win32.h>

#include <vector>

namespace Citadel::Watchtower::VLKN {
	using Keep::u32;

	Swapchain::Swapchain(const SwapchainCreateInfo& createInfo) {
		queue = createInfo.presentQueue;
		device = createInfo.device;

		VkSurfaceCapabilitiesKHR capabilities;
		{
			vkGetPhysicalDeviceSurfaceCapabilitiesKHR(createInfo.physicalDevice, createInfo.surface, &capabilities);
		}

		VkSurfaceFormatKHR surfaceFormat;
		{
			u32 count = 0;

			vkGetPhysicalDeviceSurfaceFormatsKHR(createInfo.physicalDevice, createInfo.surface, &count, nullptr);
			std::vector<VkSurfaceFormatKHR> surfaceFormats(count);
			vkGetPhysicalDeviceSurfaceFormatsKHR(createInfo.physicalDevice, createInfo.surface, &count, surfaceFormats.data());

			surfaceFormat = surfaceFormats[0];
			for (const auto& availableFormat : surfaceFormats) {
				if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
					surfaceFormat = availableFormat;
					break;
				}
			}
		}

		VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;
		{
			u32 count = 0;
			vkGetPhysicalDeviceSurfacePresentModesKHR(createInfo.physicalDevice, createInfo.surface, &count, nullptr);
			std::vector<VkPresentModeKHR> presentModes(count);
			vkGetPhysicalDeviceSurfacePresentModesKHR(createInfo.physicalDevice, createInfo.surface, &count, presentModes.data());

			for (const auto& availableMode : presentModes) {
				if (availableMode == VK_PRESENT_MODE_MAILBOX_KHR) {
					presentMode = availableMode;
					break;
				}
			}
		}

		{
			VkExtent2D extent = { createInfo.descriptor.width, createInfo.descriptor.height };
			
			VkSwapchainCreateInfoKHR swapchainCreateInfo{};
			swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
			swapchainCreateInfo.surface = createInfo.surface;
			swapchainCreateInfo.minImageCount = createInfo.descriptor.frameCount;
			swapchainCreateInfo.imageFormat = surfaceFormat.format;
			swapchainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
			swapchainCreateInfo.imageExtent = extent;
			swapchainCreateInfo.imageArrayLayers = 1;
			swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
			swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			swapchainCreateInfo.preTransform = capabilities.currentTransform;
			swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
			swapchainCreateInfo.presentMode = presentMode;
			swapchainCreateInfo.clipped = VK_TRUE;
			swapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

			vkCreateSwapchainKHR(createInfo.device, &swapchainCreateInfo, nullptr, &swapchain);
		}

		{
			VkCommandPoolCreateInfo commandPoolCreateInfo{
				VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
				nullptr,
				createInfo.presentFamily
			};
			vkCreateCommandPool(createInfo.device, &commandPoolCreateInfo, nullptr, &commandPool);
		}

		{
			VkAttachmentDescription attachments[] = {
				{
					0,
					surfaceFormat.format,
					VK_SAMPLE_COUNT_1_BIT,
					VK_ATTACHMENT_LOAD_OP_CLEAR,
					VK_ATTACHMENT_STORE_OP_STORE,
					VK_ATTACHMENT_LOAD_OP_DONT_CARE,
					VK_ATTACHMENT_STORE_OP_DONT_CARE,
					VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
					VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
				}
			};

			VkAttachmentReference attachmentReferences[] = {
				{0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL}
			};

			VkSubpassDescription subpassDescriptions[] = {
				{
					0,
					VK_PIPELINE_BIND_POINT_GRAPHICS,
					0,
					nullptr,
					1,
					attachmentReferences,
					nullptr,
					nullptr,
					0,
					nullptr
				}
			};

			VkRenderPassCreateInfo renderPassCreateInfo{
				VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
				nullptr,
				0,
				1,
				attachments,
				1,
				subpassDescriptions,
				0,
				nullptr
			};
			vkCreateRenderPass(createInfo.device, &renderPassCreateInfo, nullptr, &renderPass);
		}

		{
			u32 count = 0;
			vkGetSwapchainImagesKHR(createInfo.device, swapchain, &count, nullptr);
			images.resize(count);
			vkGetSwapchainImagesKHR(createInfo.device, swapchain, &count, images.data());

			commandBuffers.resize(count);
			VkCommandBufferAllocateInfo bufferAllocInfo{
				VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
				nullptr,
				commandPool,
				VK_COMMAND_BUFFER_LEVEL_PRIMARY,
				count
			};
			vkAllocateCommandBuffers(createInfo.device, &bufferAllocInfo, commandBuffers.data());

			for (const auto& image : images) {
				VkImageViewCreateInfo viewCreateInfo{};
				viewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
				viewCreateInfo.image = image;
				viewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
				viewCreateInfo.format = surfaceFormat.format;
				viewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				viewCreateInfo.subresourceRange.baseMipLevel = 0;
				viewCreateInfo.subresourceRange.levelCount = 1;
				viewCreateInfo.subresourceRange.baseArrayLayer = 0;
				viewCreateInfo.subresourceRange.layerCount = 1;

				VkImageView imageView;
				vkCreateImageView(createInfo.device, &viewCreateInfo, nullptr, &imageView);
				imageViews.push_back(imageView);

				VkImageView attachments[] ={ imageView };
				VkFramebufferCreateInfo framebufferInfo{};
				framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
				framebufferInfo.renderPass = renderPass;
				framebufferInfo.attachmentCount = 1;
				framebufferInfo.pAttachments = attachments;
				framebufferInfo.width = createInfo.descriptor.width;
				framebufferInfo.height = createInfo.descriptor.height;
				framebufferInfo.layers = 1;

				VkFramebuffer framebuffer;
				auto res = vkCreateFramebuffer(createInfo.device, &framebufferInfo, nullptr, &framebuffer);
				framebuffers.push_back(framebuffer);
			}
		}

		{
			VkSemaphoreCreateInfo semaphoreCreateInfo{};
			semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

			vkCreateSemaphore(createInfo.device, &semaphoreCreateInfo, nullptr, &imageAvailable);
			vkCreateSemaphore(createInfo.device, &semaphoreCreateInfo, nullptr, &renderFinished);
		}

		vkAcquireNextImageKHR(createInfo.device, swapchain, UINT64_MAX, imageAvailable, VK_NULL_HANDLE, &imageIndex);

		Record(createInfo.presentFamily);
	}

	void
	Swapchain::Swap() {
		VkSemaphore wait[] = { imageAvailable };
		VkPipelineStageFlags stages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		VkSemaphore signal[] = { renderFinished };

		VkSubmitInfo submitInfo;
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.pNext = nullptr;
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = wait;
		submitInfo.pWaitDstStageMask = stages;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffers[imageIndex];
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signal;

		vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE);

		VkSwapchainKHR swapchains[] = { swapchain };
		VkPresentInfoKHR presentInfo;
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.pNext = nullptr;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signal;
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapchains;
		presentInfo.pImageIndices = &imageIndex;
		presentInfo.pResults = nullptr;

		vkQueuePresentKHR(queue, &presentInfo);

		vkAcquireNextImageKHR(device, swapchain, UINT64_MAX, imageAvailable, VK_NULL_HANDLE, &imageIndex);
	}

	void
	Swapchain::Record(Keep::u32 queueFamily) {
		
		VkCommandBufferBeginInfo beginInfo{
			VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
			nullptr,
			VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT,
			nullptr
		};

		for (u32 i = 0; i < images.size(); ++i) {
			VkImage image = images[i];

			VkImageMemoryBarrier readyToPresent{
				VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
				nullptr,
				VK_ACCESS_TRANSFER_WRITE_BIT,
				VK_ACCESS_MEMORY_READ_BIT,
				VK_IMAGE_LAYOUT_UNDEFINED,
				VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
				queueFamily,
				queueFamily,
				image,
			};
			readyToPresent.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			readyToPresent.subresourceRange.levelCount = 1;
			readyToPresent.subresourceRange.baseMipLevel = 0;
			readyToPresent.subresourceRange.baseArrayLayer = 0;
			readyToPresent.subresourceRange.layerCount = 1;

			vkBeginCommandBuffer(commandBuffers[i], &beginInfo);
			vkCmdPipelineBarrier(commandBuffers[i], VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &readyToPresent);
			vkEndCommandBuffer(commandBuffers[i]);
		}
	}

	const std::vector<VkImageView>
	Swapchain::GetViews() {
		return imageViews;
	}
}