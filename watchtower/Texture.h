#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "keep/Types.h"

using Citadel::Keep::handle;

namespace Citadel::Watchtower {
	class Texture {
	public:
		Texture(handle data = nullptr);
		Texture(const Texture& other);
		Texture(Texture&& other) noexcept;
		~Texture() noexcept;

		Texture& operator=(const Texture& other);
		Texture& operator=(Texture&& other) noexcept;

		handle Get() { return impl; }

	private:
		handle impl;
	};
}

#endif