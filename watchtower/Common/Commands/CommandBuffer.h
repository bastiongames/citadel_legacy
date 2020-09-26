#ifndef __COMMANDBUFFER_H__
#define __COMMANDBUFFER_H__

#include "keep/Types.h"
using Citadel::Keep::handle;
using Citadel::Keep::u32;
using Citadel::Keep::real;
using Citadel::Keep::SPtr;

#include "watchtower/Common/Pipeline/PipelineState.h"
using Citadel::Watchtower::Pipeline::PipelineState;

#include "watchtower/Common/Buffers/Buffer.h"
using Citadel::Watchtower::Buffers::Buffer;

#include "watchtower/Common/Context/RenderContext.h"
using Citadel::Watchtower::Context::RenderContext;

namespace Citadel::Watchtower::Commands {
	class CommandBuffer {
	public:
		CommandBuffer(handle data = nullptr);
		CommandBuffer(const CommandBuffer& other);
		CommandBuffer(CommandBuffer&& other) noexcept;
		~CommandBuffer() noexcept;

		CommandBuffer& operator=(const CommandBuffer& other);
		CommandBuffer& operator=(CommandBuffer&& other) noexcept;

		handle Get();

		void Begin();
		void End();

		void SetViewport(real x, real y, real w, real h, real n = 0.0f, real f = 1.0f);
		void SetPipelineState(SPtr<PipelineState> pipeline);
		void SetVertexBuffer(SPtr<Buffer> vertexBuffer);
		void BeginRenderPass(SPtr<RenderContext> context);
		void Clear();
		void Draw(u32 a, u32 b);
		void EndRenderPass(SPtr<RenderContext> context);

	private:
		handle impl;
	};
}

#endif