#include "Shader.h"

Citadel::Watchtower::Shader::Shader() {
	impl = nullptr;
}

Citadel::Watchtower::Shader::Shader(std::string text, std::string entrypoint, std::string target) {
	impl = Compile(text, entrypoint, target);
}

Citadel::Watchtower::Shader::Shader(const Shader& other) {
	this->impl = other.impl;
}

Citadel::Watchtower::Shader::Shader(Shader&& other) noexcept {
	this->impl = other.impl;
	other.impl = nullptr;
}

Citadel::Watchtower::Shader::~Shader() noexcept {
	this->impl;
	this->impl = nullptr;
}

Citadel::Watchtower::Shader&
Citadel::Watchtower::Shader::operator=(const Shader& other) {
	if (this != &other) {
		this->impl = other.impl;
	}
	return *this;
}

Citadel::Watchtower::Shader&
Citadel::Watchtower::Shader::operator=(Shader&& other) noexcept {
	if (this != &other) {
		this->impl = other.impl;
		other.impl = nullptr;
	}
	return *this;
}

handle
Citadel::Watchtower::Shader::Get() {
	return impl;
}