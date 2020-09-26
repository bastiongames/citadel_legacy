#ifndef __VLKN__PIPELINELAYOUT_H__
#define __VLKN__PIPELINELAYOUT_H__

#include "watchtower/Pipelines/PipelineLayout.h"
#include "watchtower/Pipelines/PipelineLayoutDescriptor.h"

#include <vulkan/vulkan.h>

namespace Citadel::Watchtower::Pipelines::VLKN {
	class PipelineLayout : public Pipelines::PipelineLayout {
	public:
		PipelineLayout(const VkDevice& device, const PipelineLayoutDescriptor& descriptor);

	public:
		inline VkPipelineLayout GetPipelineLayout() { return pipelineLayout; }
		inline VkDescriptorSetLayout GetDescriptorSetLayout() { return descriptorSetLayout; }
		inline std::vector<VkDescriptorSetLayoutBinding> GetBindings() { return layoutBindings; }
	private:
		VkShaderStageFlags GetVkShaderStageFlags(Flags::StageFlags flags);
		VkDescriptorType GetVkDescriptorType(Flags::ResourceType flags, Flags::BindFlags bindFlags);

	private:
		VkPipelineLayout pipelineLayout;
		VkDescriptorSetLayout descriptorSetLayout;

		std::vector<VkDescriptorSetLayoutBinding> layoutBindings;
	};
}

#endif