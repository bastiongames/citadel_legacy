#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "keep/Types.h"

namespace Citadel::Watchtower::Textures {
	class Texture {
	public:
		virtual ~Texture() = default;
	};

	using PTexture = Citadel::Keep::SPtr<Texture>;
}

#endif