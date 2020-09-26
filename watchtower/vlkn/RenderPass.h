#ifndef __VLKN__RENDERPASS_H__
#define __VLKN__RENDERPASS_H__

#include "watchtower/RenderPass.h"
#include "watchtower/RenderTarget.h"
#include "Command.h"
#include <vulkan/vulkan.h>

#include <vector>

namespace Citadel::Watchtower::VLKN {
	class RenderPass : public Watchtower::RenderPass, public Command {
	public:
		RenderPass(const VkDevice& device, PRenderTarget renderTarget);

		virtual void Record(const VkCommandBuffer& commandBuffer);

	public:
		VkRenderPass GetRenderPass() { return renderPass; }
	private:
		VkRenderPass renderPass;
		std::vector<VkFramebuffer> framebuffers;
		Keep::u32 frameBufferIndex;
	};
}

#endif