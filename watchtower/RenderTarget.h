#ifndef __RENDERTARGET_H__
#define __RENDERTARGET_H__

#include "keep/Types.h"
using Citadel::Keep::u32;
using Citadel::Keep::handle;

namespace Citadel::Watchtower {
	class RenderTarget {
	public:
		RenderTarget(handle impl = nullptr);
		RenderTarget(const RenderTarget& other);
		RenderTarget(RenderTarget&& other) noexcept;
		~RenderTarget() noexcept;

		RenderTarget& operator=(const RenderTarget& other);
		RenderTarget& operator=(RenderTarget&& other) noexcept;

		handle Get() { return impl; }

	private:
		handle impl;
	};
}

#endif