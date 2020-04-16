#ifndef __VERTEXBUFFER_H__
#define __VERTEXBUFFER_H__

#include "keep/Types.h"

using Citadel::Keep::handle;

namespace Citadel::Watchtower {
	class VertexBuffer {
	public:
		VertexBuffer(handle impl = nullptr);
		VertexBuffer(const VertexBuffer& other);
		VertexBuffer(VertexBuffer&& other) noexcept;
		~VertexBuffer() noexcept;

		VertexBuffer& operator=(const VertexBuffer& other);;
		VertexBuffer& operator=(VertexBuffer&& other) noexcept;

		handle Get() { return impl; }

	private:
		handle impl;
	};
}

#endif