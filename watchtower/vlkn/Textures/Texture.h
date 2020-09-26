#ifndef __VLKN__TEXTURE_H__
#define __VLKN__TEXTURE_H__

#include "watchtower/Textures/Texture.h"
#include "watchtower/Textures/TextureDescriptor.h"

#include <vulkan/vulkan.h>

namespace Citadel::Watchtower::Textures::VLKN {
	struct DevicePair {
		VkDevice device;
		VkPhysicalDevice physicalDevice;
	};

	struct StagingBuffer {
		VkBuffer buffer;
		VkDeviceMemory memory;
	};

	class Texture : public Textures::Texture {
	public:
		Texture(const DevicePair& devices, const VkCommandBuffer& commandBuffer, const TextureDescriptor& descriptor);

		VkImageView GetView() { return view; }
		VkSampler GetSampler() { return sampler; }
	private:
		void CreateImage(const DevicePair& devices, const TextureDescriptor& descriptor);
		StagingBuffer CreateBuffer(DevicePair devices, const TextureDescriptor& descriptor, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
		void TransitionLayout(const VkCommandBuffer& commandBuffer, VkImageLayout oldLayout, VkImageLayout newLayout);

	private:
		VkImage image;
		VkDeviceMemory memory;
		VkImageView view;
		VkSampler sampler;
	};
}

#endif