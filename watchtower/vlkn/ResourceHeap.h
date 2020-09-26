#ifndef __VLKN__RESOURCEHEAP_H__
#define __VLKN__RESOURCEHEAP_H__

#include "watchtower/ResourceHeap.h"
#include "watchtower/Buffers/Buffer.h"

#include "Command.h"

#include <vulkan/vulkan.h>
#include <vector>

namespace Citadel::Watchtower::VLKN {
	class ResourceHeap : public Watchtower::ResourceHeap, public Command {
	public:
		ResourceHeap(const VkDevice& device, const ResourceHeapDescriptor& descriptor);
		void BindTexture(const Textures::PTexture texture, Keep::u32 slot);
		void BindBuffer(const Buffers::PBuffer buffer, Keep::u32 slot);

		virtual void Record(const VkCommandBuffer& commandBuffer);
	private:
		VkDescriptorPool descriptorPool;

		std::vector<VkDescriptorSet> descriptorSets;
		Keep::u32 descriptorSetIndex;

		std::vector<VkWriteDescriptorSet> writeDescriptorSets;

		// Ugh
		VkDevice device;
		VkPipelineLayout layout;
	};
}

#endif