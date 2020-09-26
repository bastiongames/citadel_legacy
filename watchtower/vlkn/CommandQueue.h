#ifndef __VLKN__COMMANDQUEUE_H__
#define __VLKN__COMMANDQUEUE_H__

#include "watchtower/CommandQueue.h"

#include <vulkan/vulkan.h>

namespace Citadel::Watchtower::VLKN {
	struct CommandQueueCreateInfo {
		VkDevice device;
		VkQueue queue;
	};

	class CommandQueue : public Watchtower::CommandQueue {
	public:
		CommandQueue(const CommandQueueCreateInfo& createInfo);
		virtual void Submit(PCommandBuffer commandBuffer);

	private:
		VkDevice device;
		VkQueue queue;
	};
}

#endif