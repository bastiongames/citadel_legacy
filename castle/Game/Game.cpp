#include "Game.h"
#include "watchtower/Common/context/RenderContext.h"

using Citadel::Keep::MakeSPtr;
using Citadel::Castle::Game;

//#include "watchtower/vlkn/Device.h"

#include <windows.h>

void Game::Setup(handle systemData) {
	//Engine level setup...
	this->impl = systemData;
	//this->renderer = MakeSPtr<Renderer>(systemData);
	HWND hwnd = *(Citadel::Keep::SPtrFromHandle<HWND>(systemData).get());
	this->device = Device::CreateDevice(hwnd);

	//Game level setup...	
	this->OnSetup();
}

void Game::Shutdown() {
	// Engine level shutdown...

	// Game level shutdown...
	this->OnShutdown();
}

void Game::Update() {
	this->updatingMiddleware.Execute([this](UpdateContext updateContext) {
		this->OnUpdate();
	}, updateContext);
}

void Game::Render() {
	this->renderingMiddleware.Execute([this](RenderContext renderContext) {
		this->OnRender();
	}, this->renderContext);
}