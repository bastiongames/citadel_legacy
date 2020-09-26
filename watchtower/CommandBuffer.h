#ifndef __COMMANDBUFFER_H__
#define __COMMANDBUFFER_H__

#include "keep/Types.h"

#include "watchtower/Pipelines/GraphicsPipeline.h"
#include "watchtower/Buffers/Buffer.h"
#include "watchtower/RenderPass.h"
#include "watchtower/ResourceHeap.h"

namespace Citadel::Watchtower {
	class CommandBuffer {
	public:
		virtual void Begin() = 0;
		virtual void SetViewport(Keep::Frustum viewport, Keep::Rect scissor) = 0;
		virtual void SetPipeline(const Pipelines::PGraphicsPipeline pipeline) = 0;
		virtual void SetVertexBuffer(const Buffers::PBuffer vertexBuffer) = 0;
		virtual void SetIndexBuffer(const Buffers::PBuffer indexBuffer) = 0;
		virtual void BeginRenderPass(const PRenderPass renderPass) = 0;
		virtual void Clear(Keep::rgba clearColor) = 0;
		virtual void SetResourceHeap(const PResourceHeap resourceHeap) = 0;
		virtual void Draw(Keep::u32 count, Keep::u32 offset) = 0;
		virtual void DrawIndexed(Keep::u32 count, Keep::u32 offset) = 0;
		virtual void EndRenderPass() = 0;

		virtual void End() = 0;
	};

	using PCommandBuffer = Citadel::Keep::SPtr<CommandBuffer>;
}

#endif