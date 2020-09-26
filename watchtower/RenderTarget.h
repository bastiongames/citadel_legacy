#ifndef __RENDERTARGET_H__
#define __RENDERTARGET_H__

#include "keep/Types.h"

namespace Citadel::Watchtower {
	struct RenderTargetDescriptor {
		Keep::u32 width;
		Keep::u32 height;
	};

	class RenderTarget {
	public:
		virtual ~RenderTarget() = default;
	};

	using PRenderTarget = Keep:: SPtr<RenderTarget>;
}

#endif