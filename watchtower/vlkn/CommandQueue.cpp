#include "CommandQueue.h"

#include "CommandBuffer.h"

namespace Citadel::Watchtower::VLKN {
	CommandQueue::CommandQueue(const CommandQueueCreateInfo& createInfo) {
		device = createInfo.device;
		queue = createInfo.queue;
	}

	void
	CommandQueue::Submit(PCommandBuffer commandBuffer) {
		Keep::SPtr<CommandBuffer> vkCommandBuffer = Keep::CastSPtr<Watchtower::CommandBuffer, VLKN::CommandBuffer>(commandBuffer);

		VkCommandBuffer commandBuffers[] = { vkCommandBuffer->GetCurrentCommandBuffer() };
		VkSubmitInfo submitInfo;
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.pNext = nullptr;
		submitInfo.waitSemaphoreCount = 0;
		submitInfo.pWaitSemaphores = nullptr;
		submitInfo.pWaitDstStageMask = 0;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = commandBuffers;
		submitInfo.signalSemaphoreCount = 0;
		submitInfo.pSignalSemaphores = nullptr;

		vkQueueSubmit(queue, 1, &submitInfo, vkCommandBuffer->GetFence());
	}
}