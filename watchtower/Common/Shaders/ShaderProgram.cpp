#include "ShaderProgram.h"

using Citadel::Watchtower::Shaders::ShaderProgram;

ShaderProgram::ShaderProgram(handle data) {
	impl = data;
}

ShaderProgram::ShaderProgram(const ShaderProgram& other) {
	this->impl = other.impl;
}

ShaderProgram::ShaderProgram(ShaderProgram&& other) noexcept {
	this->impl = other.impl;
	other.impl = nullptr;
}

ShaderProgram::~ShaderProgram() noexcept {
	this->impl;
	this->impl = nullptr;
}

ShaderProgram&
ShaderProgram::operator=(const ShaderProgram& other) {
	if (this != &other) {
		this->impl = other.impl;
	}
	return *this;
}

ShaderProgram&
ShaderProgram::operator=(ShaderProgram&& other) noexcept {
	if (this != &other) {
		this->impl = other.impl;
		other.impl = nullptr;
	}
	return *this;
}

handle
ShaderProgram::Get() {
	return impl;
}