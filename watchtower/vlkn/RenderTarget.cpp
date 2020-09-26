#include "RenderTarget.h"

namespace Citadel::Watchtower::VLKN {
	RenderTarget::RenderTarget(const RenderTargetCreateInfo& createInfo) {
		Keep::u32 numAttachments = 1;

		views.resize(numAttachments);

		std::vector<VkImageView> viewRefs(numAttachments);
		std::vector<VkFormat> colorFormats(numAttachments);

		for (int i = 0; i < numAttachments; ++i) {
			vkCreateImageView()
		}
	}
}