#include "ResourceHeap.h"
#include "Pipelines/PipelineLayout.h"

#include "Textures/Texture.h"
#include "Buffers/Buffer.h"

namespace Citadel::Watchtower::VLKN {
	ResourceHeap::ResourceHeap(const VkDevice& device, const ResourceHeapDescriptor& descriptor) {
		auto pipelineLayout = Keep::CastSPtr<Pipelines::PipelineLayout, Pipelines::VLKN::PipelineLayout>(descriptor.pipelineLayout);
		layout = pipelineLayout->GetPipelineLayout();

		std::vector<VkDescriptorPoolSize> poolSizes(descriptor.resourceCount);
		{
			const auto& bindings = pipelineLayout->GetBindings();
			for (Keep::u32 i = 0; i < descriptor.resourceCount; ++i) {
				poolSizes[i].type = bindings[i % bindings.size()].descriptorType;
				poolSizes[i].descriptorCount = 1;
			}
		}

		{
			VkDescriptorPoolCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
			createInfo.pNext = nullptr;
			createInfo.poolSizeCount = poolSizes.size();
			createInfo.pPoolSizes = poolSizes.data();
			createInfo.maxSets = 1;

			vkCreateDescriptorPool(device, &createInfo, nullptr, &descriptorPool);
		}

		{
			std::vector<VkDescriptorSetLayout> setLayouts;
			setLayouts.resize(1, pipelineLayout->GetDescriptorSetLayout());
			
			VkDescriptorSetAllocateInfo allocInfo{};
			allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			allocInfo.pNext = nullptr;
			allocInfo.descriptorPool = descriptorPool;
			allocInfo.descriptorSetCount = (Keep::u32)setLayouts.size();
			allocInfo.pSetLayouts = setLayouts.data();

			descriptorSets.resize(setLayouts.size());
			vkAllocateDescriptorSets(device, &allocInfo, descriptorSets.data());
		}

		this->device = device;
		
	}

	void
	ResourceHeap::BindTexture(const Textures::PTexture texture, Keep::u32 slot) {
		auto vkTexture = Keep::CastSPtr<Textures::Texture, Textures::VLKN::Texture>(texture);

		VkDescriptorImageInfo samplerInfo{};
		samplerInfo.imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		samplerInfo.imageView = VK_NULL_HANDLE;
		samplerInfo.sampler = vkTexture->GetSampler();

		VkDescriptorImageInfo imageInfo{};
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = vkTexture->GetView();
		imageInfo.sampler = VK_NULL_HANDLE;


		VkWriteDescriptorSet samplerWrite{};
		samplerWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		samplerWrite.pNext = nullptr;
		samplerWrite.dstSet = descriptorSets[0];
		samplerWrite.dstBinding = slot;
		samplerWrite.dstArrayElement = 0;
		samplerWrite.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
		samplerWrite.descriptorCount = 1;
		samplerWrite.pImageInfo = &samplerInfo;

		VkWriteDescriptorSet imageWrite{};
		imageWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		imageWrite.pNext = nullptr;
		imageWrite.dstSet = descriptorSets[0];
		imageWrite.dstBinding = slot+1;
		imageWrite.dstArrayElement = 0;
		imageWrite.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
		imageWrite.descriptorCount = 1;
		imageWrite.pImageInfo = &imageInfo;

		VkWriteDescriptorSet writeSets[] = { samplerWrite, imageWrite };

		vkUpdateDescriptorSets(device, 2, writeSets, 0, nullptr);
	}

	void
	ResourceHeap::BindBuffer(const Buffers::PBuffer buffer, Keep::u32 slot) {
		auto buff = Keep::CastSPtr<Buffers::Buffer, Buffers::VLKN::Buffer>(buffer);

		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = buff->GetBuffer();
		bufferInfo.offset = 0;
		bufferInfo.range = buff->GetBufferSize();

		VkWriteDescriptorSet write{};
		write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		write.pNext = nullptr;
		write.dstSet = descriptorSets[0];
		write.dstBinding = slot;
		write.dstArrayElement = 0;
		write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		write.descriptorCount = 1;
		write.pBufferInfo = &bufferInfo;

		vkUpdateDescriptorSets(device, 1, &write, 0, nullptr);
	}

	void
	ResourceHeap::Record(const VkCommandBuffer& commandBuffer) {
		//vkUpdateDescriptorSets(device, writeDescriptorSets.size(), writeDescriptorSets.data(), 0, nullptr);

		const VkDescriptorSet sets[] = { descriptorSets[0] };
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, layout, 0, 1, sets, 0, nullptr);
	}

}