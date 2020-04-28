#ifndef __CONTEXT3D_H__
#define __CONTEXT3D_H__

#include "Context.h"

namespace Citadel::Watchtower {
	class Context3D : public Context {
	public:
		Context3D(handle data = nullptr) : Context(data) {}
		Context3D(const Context3D& other): Context(other){}
		Context3D(Context3D&& other)noexcept : Context(other) {}
		~Context3D() = default;

		Context3D& operator=(const Context3D& other) {
			if (this != &other) {
				this->impl = other.impl;
			}
			return *this;
		}

		Context3D& operator=(Context3D&& other) {
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