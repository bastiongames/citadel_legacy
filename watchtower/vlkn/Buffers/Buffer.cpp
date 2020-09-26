#include "Buffer.h"

u32
findMemoryType(VkPhysicalDevice physicalDevice, Citadel::Keep::u32 filter, VkMemoryPropertyFlags properties) {
	VkPhysicalDeviceMemoryProperties memProps{};
	vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProps);
	for (u32 i = 0; i < memProps.memoryTypeCount; ++i) {
		if ((filter & (1 << i)) && (memProps.memoryTypes[i].propertyFlags & properties) == properties) {
			return i;
		}
	}
	return 0;
}

namespace Citadel::Watchtower::Buffers::VLKN {
	VkBufferUsageFlags
	GetBufferUsageFlags(const BufferDescriptor& descriptor) {
		VkBufferUsageFlags flags = VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		if ((descriptor.bindFlags & BindFlags::VertexBuffer) == BindFlags::VertexBuffer)
			flags |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		if ((descriptor.bindFlags & BindFlags::IndexBuffer) == BindFlags::IndexBuffer)
			flags |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
		if ((descriptor.bindFlags & BindFlags::ConstantBuffer) == BindFlags::ConstantBuffer)
			flags |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;

		return flags;
	}

	Buffer::Buffer(DevicePair devices, VkCommandBuffer commandBuffer, const BufferDescriptor& descriptor, void* data) {
		device = devices.device;
		size = descriptor.size;
		binding = descriptor.bindFlags;

		VkBuffer stagingBuffer;
		VkDeviceMemory deviceMemory{};
		stagingBuffer = CreateBuffer(devices, descriptor, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, deviceMemory);

		void* temp;
		vkMapMemory(devices.device, deviceMemory, 0, descriptor.size, 0, &temp);
		memcpy(temp, data, descriptor.size);
		vkUnmapMemory(devices.device, deviceMemory);

		buffer = CreateBuffer(devices, descriptor, GetBufferUsageFlags(descriptor),
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT| VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, memory);
		CopyBuffer(commandBuffer, stagingBuffer, buffer, descriptor.size);
	}

	void
	Buffer::Update(void* data, Keep::u32 size) {
		void* temp;
		vkMapMemory(device, memory, 0, size, 0, &temp);
		memcpy(temp, data, size);
		vkUnmapMemory(device, memory);
	}

	void
	Buffer::Record(const VkCommandBuffer& commandBuffer) {
		VkDeviceSize offsets[] = { 0 };
		
		if((binding & BindFlags::VertexBuffer) == BindFlags::VertexBuffer)
			vkCmdBindVertexBuffers(commandBuffer, 0, 1, &buffer, offsets);
		if ((binding & BindFlags::IndexBuffer) == BindFlags::IndexBuffer)
			vkCmdBindIndexBuffer(commandBuffer, buffer, 0, VkIndexType::VK_INDEX_TYPE_UINT16);
	}

	VkBuffer
	Buffer::CreateBuffer(DevicePair devices, const BufferDescriptor& descriptor, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkDeviceMemory& stagingMemory) {
		VkBuffer tempBuffer;

		{
			VkBufferCreateInfo bufferCreateInfo{};
			bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			bufferCreateInfo.pNext = nullptr;
			bufferCreateInfo.size = descriptor.size;
			bufferCreateInfo.usage = usage;
			bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

			vkCreateBuffer(devices.device, &bufferCreateInfo, nullptr, &tempBuffer);
		}
		
		{
			VkMemoryRequirements memoryRequirements{};
			vkGetBufferMemoryRequirements(devices.device, tempBuffer, &memoryRequirements);

			VkMemoryAllocateInfo allocateInfo{};
			allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			allocateInfo.allocationSize = memoryRequirements.size;
			allocateInfo.memoryTypeIndex = findMemoryType(devices.physicalDevice, memoryRequirements.memoryTypeBits, properties);

			vkAllocateMemory(devices.device, &allocateInfo, nullptr, &stagingMemory);
			vkBindBufferMemory(devices.device, tempBuffer, stagingMemory, 0);
		}
		return tempBuffer;
	}

	void
	Buffer::CopyBuffer(VkCommandBuffer commandBuffer, VkBuffer src, VkBuffer dst, VkDeviceSize size) {
		VkBufferCopy copyRegion{};
		copyRegion.srcOffset = 0;
		copyRegion.dstOffset = 0;
		copyRegion.size = size;
		vkCmdCopyBuffer(commandBuffer, src, dst, 1, &copyRegion);
	}
}