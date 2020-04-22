#ifndef __GAME_H__
#define __GAME_H__

#include "watchtower/Device.h"
#include "watchtower/Pipeline.h"

namespace Citadel::Castle {
	class Game {
	public:
		virtual ~Game() = default;

		void Setup(handle systemData);
		void Shutdown();

		void Update();
		void Render();

		virtual void OnSetup() = 0;
		virtual void OnShutdown() = 0;
		virtual void OnUpdate() = 0;
		virtual void OnRender() = 0;

		void SetWindowTitle(const wchar_t* title);

	protected:
		Citadel::Watchtower::Device device;
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