#include "Renderer.h"

using Citadel::Watchtower::Renderer;

Renderer::Renderer(handle platformData) {
	AcquireDevice(platformData);
}