#include "RenderTarget.h"

using Citadel::Keep::handle;

Citadel::Watchtower::RenderTarget::RenderTarget(handle impl) {
	this->impl = impl;
}

Citadel::Watchtower::RenderTarget::RenderTarget(const RenderTarget& other) {
	this->impl = other.impl;
}

Citadel::Watchtower::RenderTarget::RenderTarget(RenderTarget&& other) noexcept {
	this->impl = other.impl;
	other.impl = nullptr;
}

Citadel::Watchtower::RenderTarget::~RenderTarget() noexcept {
	//delete this->impl;
	this->impl = nullptr;
}

Citadel::Watchtower::RenderTarget&
Citadel::Watchtower::RenderTarget::operator=(const RenderTarget& other) {
	if (this != &other) {
		//delete this->impl;
		this->impl = other.impl;
	}
	return *this;
}

Citadel::Watchtower::RenderTarget&
Citadel::Watchtower::RenderTarget::operator=(RenderTarget&& other) noexcept {
	if (this != &other) {
		//delete this->impl;
		this->impl = other.impl;
		other.impl = nullptr;
	}
	return *this;
}