#ifndef __GAME3D_H__
#define __GAME3D_H__

#include "castle/Game/Game.h"
#include "keep/Types.h"
#include "watchtower/context/Context3D.h"
#include "watchtower/RenderTarget.h"

using Citadel::Keep::u32;
using Citadel::Watchtower::Context;
using Citadel::Watchtower::Context3D;
using Citadel::Watchtower::RenderTarget;

namespace Citadel::Castle {
	class Game3D : public Game {
	public:
		virtual void OnSetup() {
			context = this->device.CreateContext3D();
			target = this->device.CreateRenderTarget();
		}

		virtual void OnRender() {
			context.Begin(target, frameIndex);
			OnRender(context);
			context.Finish(target, frameIndex);

			device.Present(context);
			frameIndex = device.FinishFrame();
		}

		virtual void OnRender(Context& context) = 0;

	private:
		u32 frameIndex;
		Context3D context;
		RenderTarget target;
	};
}

#endif