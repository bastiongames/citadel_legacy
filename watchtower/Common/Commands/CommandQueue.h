#ifndef __COMMANDQUEUE_H__
#define __COMMANDQUEUE_H__

#include "keep/Types.h"
using Citadel::Keep::handle;
using Citadel::Keep::SPtr;

#include "CommandBuffer.h"

namespace Citadel::Watchtower::Commands {
	class CommandQueue {
	public:
		CommandQueue(handle data = nullptr);
		CommandQueue(const CommandQueue& other);
		CommandQueue(CommandQueue&& other) noexcept;
		~CommandQueue() noexcept;

		CommandQueue& operator=(const CommandQueue& other);
		CommandQueue& operator=(CommandQueue&& other) noexcept;

		void Submit(SPtr<CommandBuffer> buffer);

		handle Get();

	private:
		handle impl;
	};
}

#endif