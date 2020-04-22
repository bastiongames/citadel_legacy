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
	this->device.Wait();
	this->OnShutdown();
}

void Citadel::Castle::Game::Update() {
	this->OnUpdate();
}

void Citadel::Castle::Game::Render() {
	this->device.BeginFrame();

	this->OnRender();

	this->device.FinishFrame();
}