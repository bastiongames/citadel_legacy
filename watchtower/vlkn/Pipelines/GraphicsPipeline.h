#ifndef __VLKN__GRAPHICSPIPELINE_H__
#define __VLKN__GRAPHICSPIPELINE_H__

#include "watchtower/Pipelines/GraphicsPipeline.h"
#include "watchtower/Pipelines/GraphicsPipelineDescriptor.h"
#include "../Command.h"

#include <vulkan/vulkan.h>

namespace Citadel::Watchtower::Pipelines::VLKN {
	class GraphicsPipeline : public Pipelines::GraphicsPipeline, public Watchtower::VLKN::Command {
	public:
		GraphicsPipeline(VkDevice device, const GraphicsPipelineDescriptor& descriptor);

		virtual void Record(const VkCommandBuffer& commandBuffer);
	private:
		VkPipeline graphicsPipeline;
	};
}

#endif