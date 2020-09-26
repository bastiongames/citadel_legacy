#include "PipelineLayout.h"

namespace Citadel::Watchtower::Pipelines::VLKN {
	PipelineLayout::PipelineLayout(const VkDevice& device, const PipelineLayoutDescriptor& descriptor) {

		auto bindingCount = descriptor.bindings.size();
		layoutBindings.resize(bindingCount);

		for (auto i = 0; i < bindingCount; ++i) {
			layoutBindings[i].binding = descriptor.bindings[i].slot;
			layoutBindings[i].descriptorType = GetVkDescriptorType(descriptor.bindings[i].type, descriptor.bindings[i].bindFlags);
			layoutBindings[i].descriptorCount = descriptor.bindings[i].arraySize;
			layoutBindings[i].stageFlags = GetVkShaderStageFlags(descriptor.bindings[i].stageFlags);
			layoutBindings[i].pImmutableSamplers = nullptr;
		}
		
		{
			VkDescriptorSetLayoutCreateInfo descSetLayoutCreateInfo;
			descSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
			descSetLayoutCreateInfo.pNext = nullptr;
			descSetLayoutCreateInfo.flags = 0;
			descSetLayoutCreateInfo.bindingCount = (Keep::u32)layoutBindings.size();
			descSetLayoutCreateInfo.pBindings = layoutBindings.data();

			vkCreateDescriptorSetLayout(device, &descSetLayoutCreateInfo, nullptr, &descriptorSetLayout);
		}

		VkDescriptorSetLayout setLayouts[] = { descriptorSetLayout };
		{
			VkPipelineLayoutCreateInfo layoutCreateInfo{};
			layoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
			layoutCreateInfo.pNext = nullptr;
			layoutCreateInfo.flags = 0;
			layoutCreateInfo.setLayoutCount = 1;
			layoutCreateInfo.pSetLayouts = setLayouts;
			layoutCreateInfo.pushConstantRangeCount = 0;
			layoutCreateInfo.pPushConstantRanges = nullptr;
			vkCreatePipelineLayout(device, &layoutCreateInfo, nullptr, &pipelineLayout);
		}
	}

	VkShaderStageFlags
	PipelineLayout::GetVkShaderStageFlags(Flags::StageFlags flags) {
		VkShaderStageFlags bitmask = 0;
		if ((flags & Flags::StageFlags::VertexStage) == Flags::StageFlags::VertexStage)
			bitmask |= VK_SHADER_STAGE_VERTEX_BIT;
		if ((flags & Flags::StageFlags::TessControlStage) == Flags::StageFlags::TessControlStage)
			bitmask |= VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
		if ((flags & Flags::StageFlags::TessEvaluationStage) == Flags::StageFlags::TessEvaluationStage)
			bitmask |= VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
		if ((flags & Flags::StageFlags::GeometryStage) == Flags::StageFlags::GeometryStage)
			bitmask |= VK_SHADER_STAGE_GEOMETRY_BIT;
		if ((flags & Flags::StageFlags::FragmentStage) == Flags::StageFlags::FragmentStage)
			bitmask |= VK_SHADER_STAGE_FRAGMENT_BIT;
		if ((flags & Flags::StageFlags::ComputeStage) == Flags::StageFlags::ComputeStage)
			bitmask |= VK_SHADER_STAGE_COMPUTE_BIT;

		return bitmask;
	}

	VkDescriptorType
	PipelineLayout::GetVkDescriptorType(Flags::ResourceType flags, Flags::BindFlags bindFlags) {
		switch (flags) {
		case Flags::ResourceType::Sampler:
			return VK_DESCRIPTOR_TYPE_SAMPLER;
		case Flags::ResourceType::Texture:
			//return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			return VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
		case Flags::ResourceType::Buffer:
			if ((bindFlags & Flags::BindFlags::ConstantBuffer) == Flags::BindFlags::ConstantBuffer)
				return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			if ((bindFlags & (Flags::BindFlags::Sampled | Flags::BindFlags::Storage)) != Flags::BindFlags::None)
				return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
			break;
		}
		return VK_DESCRIPTOR_TYPE_MAX_ENUM;
	}
}