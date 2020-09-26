#ifndef __BUFFER_H__
#define __BUFFER_H__

#include "keep/Types.h"

using Citadel::Keep::handle;

namespace Citadel::Watchtower::Buffers {
	class Buffer {
	public:
		Buffer(handle data = nullptr);
		Buffer(const Buffer& buffer);
		Buffer(Buffer&& buffer) noexcept;
		~Buffer() noexcept;

		Buffer& operator=(const Buffer& buffer);
		Buffer& operator=(Buffer&& buffer) noexcept;

		handle Get() { return impl; }

	private:
		handle impl;
	};
}

#endif