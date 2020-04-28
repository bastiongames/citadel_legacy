#include "Pipeline.h"

Citadel::Watchtower::Pipeline::Pipeline(handle impl) {
	this->impl = impl;
}

Citadel::Watchtower::Pipeline::Pipeline(const Pipeline& other) {
	this->impl = other.impl;
}

Citadel::Watchtower::Pipeline::Pipeline(Pipeline&& other) noexcept {
	this->impl = other.impl;
	other.impl = nullptr;
}

Citadel::Watchtower::Pipeline::~Pipeline() noexcept {
	this->impl = nullptr;
}

Citadel::Watchtower::Pipeline&
Citadel::Watchtower::Pipeline::operator=(const Pipeline& other) {
	if (this != &other) {
		this->impl = other.impl;
	}
	return *this;
}

Citadel::Watchtower::Pipeline&
Citadel::Watchtower::Pipeline::operator=(Pipeline&& other) noexcept {
	if (this != &other) {
		this->impl = other.impl;
		other.impl = nullptr;
	}
	return *this;
}