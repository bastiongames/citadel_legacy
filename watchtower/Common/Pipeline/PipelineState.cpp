#include "PipelineState.h"

using Citadel::Watchtower::Pipeline::PipelineState;

PipelineState::PipelineState(handle data) {
	impl = data;
}

PipelineState::PipelineState(const PipelineState& other) {
	this->impl = other.impl;
}

PipelineState::PipelineState(PipelineState&& other) noexcept {
	this->impl = other.impl;
	other.impl = nullptr;
}

PipelineState::~PipelineState() noexcept {
	this->impl;
	this->impl = nullptr;
}

PipelineState&
PipelineState::operator=(const PipelineState& other) {
	if (this != &other) {
		this->impl = other.impl;
	}
	return *this;
}

PipelineState&
PipelineState::operator=(PipelineState&& other) noexcept {
	if (this != &other) {
		this->impl = other.impl;
		other.impl = nullptr;
	}
	return *this;
}

handle
PipelineState::Get() {
	return impl;
}