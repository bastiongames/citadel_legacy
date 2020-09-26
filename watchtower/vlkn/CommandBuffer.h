#ifndef __VLKN__COMMANDBUFFER_H__
#define __VLKN__COMMANDBUFFER_H__

#include "watchtower/CommandBuffer.h"

#include <vulkan/vulkan.h>
#include <vector>

namespace Citadel::Watchtower::VLKN {
	struct CommandBufferCreateInfo {
		VkPhysicalDevice physicalDevice;
		VkDevice device;
		VkQueue queue;
		Keep::u32 queueFamily;
	};

	class CommandBuffer : public Watchtower::CommandBuffer {
	public:
		CommandBuffer(const CommandBufferCreateInfo& createInfo);
		virtual void Begin();
		virtual void SetViewport(Keep::Frustum viewport, Keep::Rect scissor);
		virtual void SetPipeline(const Pipelines::PGraphicsPipeline pipeline);
		virtual void SetVertexBuffer(const Buffers::PBuffer vertexBuffer);
		virtual void SetIndexBuffer(const Buffers::PBuffer indexBuffer);
		virtual void BeginRenderPass(const PRenderPass renderPass);
		virtual void Clear(Keep::rgba clearColor);
		virtual void SetResourceHeap(const PResourceHeap resourceHeap);
		virtual void Draw(Keep::u32 count, Keep::u32 offset);
		virtual void DrawIndexed(Keep::u32 cout, Keep::u32 offset);
		virtual void EndRenderPass();

		virtual void End();

	public:
		VkCommandBuffer GetCurrentCommandBuffer() { return commandBuffer; }
		VkFence GetFence() { return fence; }

	private:
		VkCommandPool commandPool;
		std::vector<VkCommandBuffer> commandBuffers;
		VkCommandBuffer commandBuffer;
		Keep::u32 commandBufferIndex;

		std::vector<VkFence> fences;
		VkFence fence;
		VkDevice device;
	};
}

#endif