#include "Game.h"

void Citadel::Castle::Game::Setup(handle systemData) {
	//Engine level setup...
	this->device = Citadel::Watchtower::Device(systemData);
	this->impl = systemData;

	this->device.Wait();

	//Game level setup...	
	this->OnSetup();
}

void Citadel::Castle::Game::Shutdown() {
	// Wait for all GPU execution to complete before exiting.
	this->device.Wait();
	// Engine level shutdown...

	// Game level shutdown...
	this->OnShutdown();
}

void Citadel::Castle::Game::Update() {
	// Engine level update...

	// Game level update...
	this->OnUpdate();
}

void Citadel::Castle::Game::Render() {
	// Engine level rendering...

	// Game level rendering...
	this->OnRender();
}