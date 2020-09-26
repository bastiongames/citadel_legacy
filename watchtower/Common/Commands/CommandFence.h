#ifndef __COMMANDFENCE_H__
#define __COMMANDFENCE_H__


#include "keep/Types.h"
using Citadel::Keep::handle;
using Citadel::Keep::u64;

namespace Citadel::Watchtower::Commands {
	class CommandFence {
	public:
		CommandFence(handle data = nullptr);
		CommandFence(const CommandFence& other);
		CommandFence(CommandFence&& other) noexcept;
		~CommandFence() noexcept;

		CommandFence& operator=(const CommandFence& other);
		CommandFence& operator=(CommandFence&& other) noexcept;

		bool Wait(u64 timeoutNS) { return false; }
		bool WaitForValue(u64 value, u64 timeoutMS = ~0) { return false; }
		bool WaitForValueAndUpdate(u64& value, u64 timeoutMS = ~0) { return false; }

		handle Get();

	private:
		handle impl;
	};
}

#endif