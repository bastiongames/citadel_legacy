#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "keep/Types.h"
using Citadel::Keep::handle;

#include "watchtower/Pipeline.h"
#include "watchtower/RenderTarget.h"
#include "watchtower/Texture.h"

namespace Citadel::Watchtower {
	class Context {
	public:
		Context(handle data = nullptr) {
			impl = data;
		}
		Context(const Context& other) {
			this->impl = other.impl;
		}
		Context(Context&& other) noexcept {
			this->impl = other.impl;
			other.impl = nullptr;
		}
		~Context() {
			this->impl = nullptr;
		}

		Context& operator=(const Context& other) {
			if (this != &other) {
				this->impl = other.impl;
			}
			return *this;
		}

		Context& operator=(Context&& other) noexcept {
			if (this != &other) {
				this->impl = other.impl;
				other.impl = nullptr;
			}
			return *this;
		}

		virtual Context& Begin(RenderTarget& target, u32 frameIndex) = 0;

		virtual Context& UsingPipeline(Pipeline& pipeline) = 0;
		virtual Context& PaintWith(Texture& texture) = 0;

		virtual Context& DrawTriangleList(VertexBuffer& buffer) = 0;

		virtual Context& Finish(RenderTarget& target, u32 frameIndex) = 0;

		handle Get() { return impl; }

	protected:
		handle impl;
	};
}

#endif