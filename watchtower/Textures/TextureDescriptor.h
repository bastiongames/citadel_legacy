#ifndef __TEXTUREDESCRIPTOR_H__
#define __TEXTUREDESCRIPTOR_H__

#include "keep/Types.h"

namespace Citadel::Watchtower::Textures {
	struct TextureDescriptor {
		Keep::u32 width;
		Keep::u32 height;

		Keep::handle data;
	};
}

#endif