#ifndef __RENDERCONTEXT_H__
#define __RENDERCONTEXT_H__

#include "keep/Types.h"
using Citadel::Keep::handle;

namespace Citadel::Watchtower::Context {
	class RenderContext {
	public:
		RenderContext(handle data = nullptr);
		RenderContext(const RenderContext& context);
		RenderContext(RenderContext&& context) noexcept;
		~RenderContext() noexcept;

		RenderContext& operator=(const RenderContext& context);
		RenderContext& operator=(RenderContext&& context) noexcept;

		void Present();

		handle Get() { return impl; }

	private:
		handle impl;
	};
}

#endif