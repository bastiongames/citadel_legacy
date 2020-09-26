#ifndef __BUFFER_H__
#define __BUFFER_H__

#include "keep/Types.h"

namespace Citadel::Watchtower::Buffers {
	class Buffer {
	public:
		virtual void Update(void* data, Keep::u32 size) = 0;
	private:
	};

	using PBuffer = Keep::SPtr<Buffer>;
}

#endif