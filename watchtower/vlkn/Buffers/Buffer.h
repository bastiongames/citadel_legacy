#ifndef __VLKN__BUFFER_H__
#define __VLKN__BUFFER_H__

#include "watchtower/Buffers/Buffer.h"
#include "watchtower/Buffers/BufferDescriptor.h"
#include "../Command.h"

#include <vulkan/vulkan.h>

namespace Citadel::Watchtower::Buffers::VLKN {
	struct DevicePair {
		VkDevice device;
		VkPhysicalDevice physicalDevice;
	};

	class Buffer : public Buffers::Buffer, public Watchtower::VLKN::Command {
	public:
		Buffer(DevicePair devicePair, VkCommandBuffer commandBuffer, const BufferDescriptor& descriptor, void* data);

		void Update(void* data, Keep::u32 size);

	public:
		VkBuffer GetBuffer() { return buffer; }
		Keep::u32 GetBufferSize() { return size; }
	private:
		VkBuffer CreateBuffer(DevicePair devices, const BufferDescriptor& descriptor, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkDeviceMemory& stagingMemory);
		void CopyBuffer(VkCommandBuffer commandBuffer, VkBuffer src, VkBuffer dst, VkDeviceSize size);
		void Record(const VkCommandBuffer& commandBuffer);

	private:
		VkBuffer buffer;
		VkDeviceMemory memory;
		Keep::u32 size;
		BindFlags binding;

		VkDevice device;
	};
}

#endif