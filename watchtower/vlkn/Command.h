#ifndef __VLKN__COMMAND_H__
#define __VLKN__COMMAND_H__

#include <vulkan/vulkan.h>

namespace Citadel::Watchtower::VLKN {
	class Command {
	public:
		virtual void Record(const VkCommandBuffer& commandBuffer) = 0;
	};
}

#endif