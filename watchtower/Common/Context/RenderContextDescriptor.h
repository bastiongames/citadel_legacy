#ifndef __RENDERCONTEXTDESCRIPTOR_H__
#define __RENDERCONTEXTDESCRIPTOR_H__

#include "keep/Types.h"
using Citadel::Keep::u32;

namespace Citadel::Watchtower::Context {
	struct RenderContextDescriptor {
		u32 width;
		u32 height;
	};
}

#endif
