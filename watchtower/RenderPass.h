#ifndef __RENDERPASS_H__
#define __RENDERPASS_H__

#include "keep/Types.h"

namespace Citadel::Watchtower {
	class RenderPass {
	public:
		virtual ~RenderPass() = default;
	};

	using PRenderPass = Keep::SPtr<RenderPass>;
}

#endif