#include "Texture.h"

Citadel::Watchtower::Texture::Texture(handle data) {
	impl = data;
}

Citadel::Watchtower::Texture::Texture(const Texture& other) {
	impl = other.impl;
}

Citadel::Watchtower::Texture::Texture(Texture&& other) noexcept {
	this->impl = other.impl;
	other.impl = nullptr;
}

Citadel::Watchtower::Texture::~Texture() noexcept {
	this->impl = nullptr;
}

Citadel::Watchtower::Texture&
Citadel::Watchtower::Texture::operator=(const Texture& other) {
	if (this != &other) {
		this->impl = other.impl;
	}

	return *this;
}

Citadel::Watchtower::Texture&
Citadel::Watchtower::Texture::operator=(Texture&& other) noexcept {
	if (this != &other) {
		this->impl = other.impl;
		other.impl = nullptr;
	}
	return *this;
}