#ifndef __VLKN_RENDERTARGET_H__
#define __VLKN_RENDERTARGET_H__

#include <vulkan/vulkan.h>

namespace Citadel::Watchtower::Vlkn {
	struct RenderTargetData {
		//VkRenderPass renderPass;
		std::vector<VkFramebuffer> framebuffers;
	};
}

#endif