#ifndef __DX12__RENDERPASS_H__
#define __DX12__RENDERPASS_H__

#include "watchtower/RenderPass.h"
#include "RenderTarget.h"

#include <d3d12.h>
#include <wrl/client.h>
using Microsoft::WRL::ComPtr;

namespace Citadel::Watchtower::DX12 {
	class RenderPass : public Watchtower::RenderPass {
	public:
		RenderPass(const PRenderTarget renderTarget);

	public:
		PRenderTarget GetTarget();

	private:
		Keep::SPtr<DX12::RenderTarget> renderTarget;
	};
}

#endif