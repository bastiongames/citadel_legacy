#include "Shader.h"

using Citadel::Watchtower::Shaders::Shader;

Shader::Shader(handle data) {
	impl = data;
}

Shader::Shader(const Shader& other) {
	this->impl = other.impl;
}

Shader::Shader(Shader&& other) noexcept {
	this->impl = other.impl;
	other.impl = nullptr;
}

Shader::~Shader() noexcept {
	this->impl;
	this->impl = nullptr;
}

Shader&
Shader::operator=(const Shader& other) {
	if (this != &other) {
		this->impl = other.impl;
	}
	return *this;
}

Shader&
Shader::operator=(Shader&& other) noexcept {
	if (this != &other) {
		this->impl = other.impl;
		other.impl = nullptr;
	}
	return *this;
}

handle
Shader::Get() {
	return impl;
}