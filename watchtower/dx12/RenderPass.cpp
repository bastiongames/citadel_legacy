#include "RenderPass.h"
#include "RenderTarget.h"

namespace Citadel::Watchtower::DX12 {
	RenderPass::RenderPass(const PRenderTarget renderTarget) {
		auto target = Keep::CastSPtr<Watchtower::RenderTarget, DX12::RenderTarget>(renderTarget);

		this->renderTarget = target;
	}

	PRenderTarget
	RenderPass::GetTarget() {
		return renderTarget;
	}
}