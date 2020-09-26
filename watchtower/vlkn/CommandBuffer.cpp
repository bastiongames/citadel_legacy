#include "CommandBuffer.h"
#include "RenderPass.h"

namespace Citadel::Watchtower::VLKN {

	CommandBuffer::CommandBuffer(const CommandBufferCreateInfo& createInfo) {
		Keep::u32 bufferCount = 1;
		device = createInfo.device;

		{
			VkCommandPoolCreateInfo commandPoolCreateInfo;
			commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
			commandPoolCreateInfo.pNext = nullptr;
			commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
			commandPoolCreateInfo.queueFamilyIndex = createInfo.queueFamily;

			vkCreateCommandPool(createInfo.device, &commandPoolCreateInfo, nullptr, &commandPool);
		}

		{
			commandBuffers.resize(bufferCount);
			VkCommandBufferAllocateInfo allocInfo;
			allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			allocInfo.pNext = nullptr;
			allocInfo.commandPool = commandPool;
			allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
			allocInfo.commandBufferCount = bufferCount;

			vkAllocateCommandBuffers(createInfo.device, &allocInfo, commandBuffers.data());
		}
	
		{
			fences.resize(bufferCount);

			VkFenceCreateInfo fenceCreateInfo;
			fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
			fenceCreateInfo.pNext = nullptr;
			fenceCreateInfo.flags = 0;

			for (Keep::u32 i = 0; i < bufferCount; ++i) {
				vkCreateFence(createInfo.device, &fenceCreateInfo, nullptr, &fences[i]);
				vkQueueSubmit(createInfo.queue, 0, nullptr, fences[i]);
			}
		}

		fence = fences[0];
		commandBuffer = commandBuffers[0];
		commandBufferIndex = 0;
	}

	void
	CommandBuffer::Begin() {
		vkWaitForFences(device, 1, &fence, VK_TRUE, UINT64_MAX);
		vkResetFences(device, 1, &fence);

		VkCommandBufferBeginInfo beginInfo;
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.pNext = nullptr;
		beginInfo.flags = 0;
		beginInfo.pInheritanceInfo = nullptr;

		vkBeginCommandBuffer(commandBuffer, &beginInfo);
	}

	void
	CommandBuffer::SetViewport(Keep::Frustum viewport, Keep::Rect scissor) {
		VkViewport viewports[] = {
			//{viewport.x, viewport.y, viewport.w, viewport.h, viewport.n, viewport.f}
			{
				0,
				720,
				1280,
				-720
			}
		};

		VkViewport v{};
		v.width = viewport.w;
		v.height = -viewport.h;
		v.x = 0;
		v.y = viewport.h;

		vkCmdSetViewport(commandBuffer, 0, 1, &v);// viewports);

		VkRect2D scissors[] = {
			{(Keep::u64)scissor.x, (Keep::u64)scissor.y, (Keep::u64)scissor.w, (Keep::u64)scissor.h}
		};

		vkCmdSetScissor(commandBuffer, 0, 1, scissors);
	}

	void
	CommandBuffer::SetPipeline(const Pipelines::PGraphicsPipeline pipeline) {
		auto command = Keep::CastSPtr<Watchtower::Pipelines::GraphicsPipeline, Command>(pipeline);
		command->Record(commandBuffer);
	}

	void
	CommandBuffer::SetVertexBuffer(const Buffers::PBuffer vertexBuffer) {
		auto command = Keep::CastSPtr<Watchtower::Buffers::Buffer, Command>(vertexBuffer);
		command->Record(commandBuffer);
	}

	void
	CommandBuffer::SetIndexBuffer(const Buffers::PBuffer indexBuffer) {
		auto command = Keep::CastSPtr<Watchtower::Buffers::Buffer, Command>(indexBuffer);
		command->Record(commandBuffer);
	}


	void
	CommandBuffer::BeginRenderPass(const PRenderPass renderPass) {
		auto renderpass = Keep::CastSPtr<Watchtower::RenderPass, Command>(renderPass);
		renderpass->Record(commandBuffer);
	}

	void
	CommandBuffer::Clear(Keep::rgba clearColor) {

		VkClearAttachment attachments[1];
		attachments[0].aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		attachments[0].colorAttachment = 0;
		attachments[0].clearValue.color = { clearColor.r, clearColor.g, clearColor.b, clearColor.a };

		VkClearRect clearRect{};
		clearRect.rect.offset.x = 0;
		clearRect.rect.offset.y = 0;
		clearRect.rect.extent = { 1280, 720 };
		clearRect.baseArrayLayer = 0;
		clearRect.layerCount = 1;

		vkCmdClearAttachments(commandBuffer, 1, attachments, 1, &clearRect);
	}

	void
	CommandBuffer::SetResourceHeap(const PResourceHeap resourceHeap) {
		auto command = Keep::CastSPtr<ResourceHeap, Command>(resourceHeap);
		command->Record(commandBuffer);
	}

	void
	CommandBuffer::Draw(Keep::u32 count, Keep::u32 offset) {
		vkCmdDraw(commandBuffer, count, 1, offset, 0);
	}
	void
	CommandBuffer::DrawIndexed(Keep::u32 count, Keep::u32 offset) {
		vkCmdDrawIndexed(commandBuffer, count, 1, offset, 0, 0);
	}
	void
	CommandBuffer::EndRenderPass() {
		vkCmdEndRenderPass(commandBuffer);
	}

	void
	CommandBuffer::End() {
		vkEndCommandBuffer(commandBuffer);
		commandBufferIndex = ((Keep::u64)commandBufferIndex + 1) % commandBuffers.size();
		commandBuffer = commandBuffers[commandBufferIndex];
		fence = fences[commandBufferIndex];
	}
}