#include "RenderPass.h"
#include "RenderTarget.h"

#include <vector>

namespace Citadel::Watchtower::VLKN {
	RenderPass::RenderPass(const VkDevice& device, const PRenderTarget renderTarget) {
		auto target = Keep::CastSPtr<Watchtower::RenderTarget, VLKN::RenderTarget>(renderTarget);

		std::vector<VkAttachmentDescription> attachments(1);

		for (auto& attachment : attachments) {
			attachment.flags = 0;
			attachment.samples = VK_SAMPLE_COUNT_1_BIT;
			attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
			attachment.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
			attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			attachment.format = VK_FORMAT_B8G8R8A8_UNORM;
		}

		VkAttachmentReference colorAttachmentReferences[] = {
			{
				0,
				VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
			}
		};

		VkSubpassDescription subpassDescriptions[] = {
			{
				0,
				VK_PIPELINE_BIND_POINT_GRAPHICS,
				0,
				nullptr,
				1,
				colorAttachmentReferences,
				nullptr,
				nullptr,
				0,
				nullptr
			}
		};

		VkRenderPassCreateInfo renderPassCreateInfo;
		renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassCreateInfo.pNext = nullptr;
		renderPassCreateInfo.attachmentCount = (Keep::u32)attachments.size();
		renderPassCreateInfo.pAttachments = attachments.data();
		renderPassCreateInfo.subpassCount = 1;
		renderPassCreateInfo.pSubpasses = subpassDescriptions;
		renderPassCreateInfo.dependencyCount = 0;
		renderPassCreateInfo.pDependencies = nullptr;
		renderPassCreateInfo.flags = 0;
		

		auto result = vkCreateRenderPass(device, &renderPassCreateInfo, nullptr, &renderPass);

		auto views = target->GetViews();
		framebuffers.resize(views.size());
		int i = 0;
		for (auto view : views) {
			VkFramebufferCreateInfo framebufferCreateInfo{
				VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
				nullptr,
				0,
				renderPass,
				1,
				&view,
				1280, 720,
				1
			};
			vkCreateFramebuffer(device, &framebufferCreateInfo, nullptr, &framebuffers[i]);
			++i;
		}

		frameBufferIndex = 0;
	}

	void
	RenderPass::Record(const VkCommandBuffer& commandBuffer) {
		VkRenderPassBeginInfo beginInfo;
		beginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		beginInfo.pNext = nullptr;
		beginInfo.renderPass = renderPass;
		beginInfo.framebuffer = framebuffers[frameBufferIndex];
		beginInfo.renderArea.offset = { 0,0 };
		beginInfo.renderArea.extent = { 1280,720 };
		beginInfo.clearValueCount = 0;

		vkCmdBeginRenderPass(commandBuffer, &beginInfo, VK_SUBPASS_CONTENTS_INLINE);

		frameBufferIndex = ((Keep::u64)frameBufferIndex + 1) % framebuffers.size();
	}
}