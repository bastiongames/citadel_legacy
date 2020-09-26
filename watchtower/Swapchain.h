#ifndef __SWAPCHAIN_H__
#define __SWAPCHAIN_H__

#include "watchtower/RenderTarget.h"

#include "keep/Types.h"

namespace Citadel::Watchtower {
	class Swapchain : public RenderTarget {
	public:
		virtual void Swap() = 0;
	};

	using PSwapchain = Citadel::Keep::SPtr<Swapchain>;
}

#endif