#ifndef __HELLOCUBE_H__
#define __HELLOCUBE_H__

#include "castle/game/Game.h"

#include "watchtower/Buffers/Buffer.h"
#include "watchtower/Shaders/Shader.h"
#include "watchtower/Textures/Texture.h"
#include "watchtower/ResourceHeap.h"

#include "abacus/Matrix/Matrix4x4.h"

struct MVP {
	Citadel::Abacus::Matrix::Matrix4x4 model;
	Citadel::Abacus::Matrix::Matrix4x4 view;
	Citadel::Abacus::Matrix::Matrix4x4 proj;
};

class HelloCube : public Citadel::Castle::Game {
public:
	void OnSetup();
	void OnShutdown();
	void OnUpdate();
	void OnRender();

private:
	Citadel::Watchtower::PSwapchain swapchain;
	Citadel::Watchtower::PCommandQueue commandQueue;
	Citadel::Watchtower::PCommandBuffer commandBuffer;
	Citadel::Watchtower::PRenderPass renderPass;
	Citadel::Watchtower::Buffers::PBuffer vertexBuffer;
	Citadel::Watchtower::Buffers::PBuffer indexBuffer;
	Citadel::Watchtower::Buffers::PBuffer constantBuffer;
	Citadel::Watchtower::Pipelines::PGraphicsPipeline graphicsPipeline;
	Citadel::Watchtower::PResourceHeap resourceHeap;

	MVP mvp;

	Citadel::Keep::real xTheta = 0;
	Citadel::Keep::real yTheta = 0;
	Citadel::Keep::real zTheta = 0;
};

#endif