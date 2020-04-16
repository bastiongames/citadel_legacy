#ifndef __PIPELINE_H__
#define __PIPELINE_H__

#include <keep\Types.h>
using Citadel::Keep::handle;

#include "watchtower/RenderTarget.h"
#include "watchtower/VertexBuffer.h"

namespace Citadel::Watchtower {
	class Pipeline
	{
	public:
		Pipeline(handle impl = nullptr);
		Pipeline(const Pipeline& other);
		Pipeline(Pipeline&& other) noexcept;
		~Pipeline() noexcept;

		Pipeline& operator=(const Pipeline& other);
		Pipeline& operator=(Pipeline&& other) noexcept;

		void Reset();

		handle Get() { return impl; }

		Pipeline& Draw(VertexBuffer buffer);
		Pipeline& Finish();
		
	private:
		handle impl;
	};
}
#endif