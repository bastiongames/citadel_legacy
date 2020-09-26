#ifndef __GAME_H__
#define __GAME_H__

#include <optional>

#include "keep/Types.h"
#include "keep/Middleware/MiddlewareEngine.h"
#include "watchtower/Common/Context/RenderContext.h"
#include "watchtower/Device.h"

using Citadel::Keep::handle;
using Citadel::Keep::SPtr;
using Citadel::Keep::Middleware::MiddlewareEngine;

using Citadel::Watchtower::Device;

namespace Citadel::Castle {
	struct UpdateContext { };
	struct RenderContext {};
	
	class Game {
	public:
		virtual ~Game() = default;

		void Setup(handle systemData);
		void Shutdown();

		void Update();
		void Render();

		virtual void OnSetup() = 0;
		virtual void OnShutdown() = 0;
		virtual void OnUpdate()= 0;
		virtual void OnRender() = 0;

		void SetWindowTitle(const wchar_t* title);

	protected:
		MiddlewareEngine<UpdateContext> updatingMiddleware;
		MiddlewareEngine<RenderContext> renderingMiddleware;

		UpdateContext updateContext;
		RenderContext renderContext;
		SPtr<Device> device;

		handle impl;
	};

	class GameMaker {
	public:
		static Game* Get();
	};

	#define REGISTER_GAME(game) \
		Citadel::Castle::Game* Citadel::Castle::GameMaker::Get() { return new game(); }

	#define REGISTER_GAME_TYPE(GameType) REGISTER_GAME(##GameType)

}

#endif