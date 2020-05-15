#include "../context/Context2D.h"
#include "ContextData.h"
#include "RenderTargetData.h"
#include "PipelineData.h"
#include "VertexBufferData.h"

using Citadel::Watchtower::Context;
using Citadel::Watchtower::Context2D;
using Citadel::Watchtower::Vlkn::ContextData;
using Citadel::Watchtower::Vlkn::RenderTargetData;
using Citadel::Watchtower::Vlkn::PipelineData;
using Citadel::Watchtower::Vlkn::VertexBufferData;

using Citadel::Keep::SPtrFromHandle;

Context&
Context2D::Begin(RenderTarget& target, u32 frameIndex) {
	auto contextData = SPtrFromHandle<ContextData>(impl);
	auto renderTargetData = SPtrFromHandle<RenderTargetData>(target.Get());
	VkResult result;

	contextData->currentFrame = frameIndex;
	VkCommandBuffer commandBuffer = contextData->commandBuffers[contextData->currentFrame];

	result = vkWaitForFences(contextData->device, 1, &contextData->consumedFences[contextData->currentFrame], VK_TRUE, UINT64_MAX);

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = 0;
	beginInfo.pInheritanceInfo = nullptr;
	result = vkBeginCommandBuffer(commandBuffer, &beginInfo);

	return *this;
}

Context&
Context2D::UsingPipeline(Pipeline& pipeline) {
	auto contextData = SPtrFromHandle<ContextData>(impl);
	auto pipelineData = SPtrFromHandle<PipelineData>(pipeline.Get());
	
	VkCommandBuffer commandBuffer = contextData->commandBuffers[contextData->currentFrame];

	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineData->graphicsPipeline);

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = pipelineData->renderPass;
	renderPassInfo.framebuffer = pipelineData->framebuffers[contextData->currentFrame];
	renderPassInfo.renderArea.offset = { 0,0 };
	renderPassInfo.renderArea.extent = { 1280, 720 };

	VkClearValue clearColor = { 0.0f, 0.2f,0.4f, 1.0f };
	renderPassInfo.clearValueCount = 1;
	renderPassInfo.pClearValues = &clearColor;

	
	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	return *this;
}

Context&
Context2D::PaintWith(Texture& texture) {
	VkWriteDescriptorSet writeDescriptor{};
	writeDescriptor.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writeDescriptor.pNext = nullptr;
	writeDescriptor.

	return *this;
}

Context&
Context2D::DrawTriangleList(VertexBuffer& buffer) {
	auto contextData = SPtrFromHandle<ContextData>(impl);
	auto vertexBufferData = SPtrFromHandle<VertexBufferData>(buffer.Get());

	auto commandBuffer = contextData->commandBuffers[contextData->currentFrame];

	VkBuffer buffers[] = { vertexBufferData->buffer };
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);

	vkCmdDraw(commandBuffer, 3, 1, 0, 0);

	return *this;
}

Context&
Context2D::Finish(RenderTarget& target, u32 frameIndex) {
	auto contextData = SPtrFromHandle<ContextData>(impl);

	vkCmdEndRenderPass(contextData->commandBuffers[frameIndex]);
	vkEndCommandBuffer(contextData->commandBuffers[frameIndex]);
	return *this;
}