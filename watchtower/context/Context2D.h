#ifndef __CONTEXT2D_H__
#define __CONTEXT2D_H__

#include "Context.h"

namespace Citadel::Watchtower {
	class Context2D : public Context {
	public:
		Context2D(handle data = nullptr) : Context(data) {}
		Context2D(const Context2D& other): Context(other){}
		Context2D(Context2D&& other)noexcept : Context(other) {}
		~Context2D() = default;

		Context2D& operator=(const Context2D& other) {
			if (this != &other) {
				this->impl = other.impl;
			}
			return *this;
		}

		Context2D& operator=(Context2D&& other) {
			if (this != &other) {
				this->impl = other.impl;
				other.impl = nullptr;
			}
			return *this;
		}

		virtual Context& Begin(RenderTarget& target, u32 frameIndex);

		virtual Context& UsingPipeline(Pipeline& pipeline);
		virtual Context& PaintWith(Texture& texture);

		virtual Context& DrawTriangleList(VertexBuffer& buffer);

		virtual Context& Finish(RenderTarget& target, u32 frameIndex);
	};
}

#endif