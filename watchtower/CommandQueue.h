#ifndef __COMMANDQUEUE_H__
#define __COMMANDQUEUE_H__

#include "keep/Types.h"
#include "watchtower/CommandBuffer.h"

namespace Citadel::Watchtower {
	class CommandQueue {
	public:
		virtual void Submit(PCommandBuffer commandBuffer) = 0;
	};

	using PCommandQueue = Citadel::Keep::SPtr<CommandQueue>;
}

#endif