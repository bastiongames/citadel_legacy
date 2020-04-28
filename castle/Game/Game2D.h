#ifndef __GAME2D_H__
#define __GAME2D_H__

#include "castle/Game/Game.h"
#include "keep/Types.h"
#include "watchtower/context/Context2D.h"
#include "watchtower/RenderTarget.h"

using Citadel::Keep::u32;
using Citadel::Watchtower::Context;
using Citadel::Watchtower::Context2D;
using Citadel::Watchtower::RenderTarget;

namespace Citadel::Castle {
	class Game2D : public Game {
	public:
		virtual void OnSetup() {
			context = this->device.CreateContext2D();
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

	protected:
		u32 frameIndex;
		Context2D context;
		RenderTarget target;
	};
}

#endif