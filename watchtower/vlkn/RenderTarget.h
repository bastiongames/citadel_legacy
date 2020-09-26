#ifndef __VLKN__RENDERTARGET_H__
#define __VLKN__RENDERTARGET_H__

#include "watchtower/RenderTarget.h"

#include <vulkan/vulkan.h>
#include <vector>

namespace Citadel::Watchtower::VLKN {
	struct RenderTargetCreateInfo {
		RenderTargetDescriptor descriptor;
		VkDevice device;
	};

	class RenderTarget : public Watchtower::RenderTarget {
	public:	
	public:
		virtual const std::vector<VkImageView> GetViews() = 0;

	private:

	};
}

#endif