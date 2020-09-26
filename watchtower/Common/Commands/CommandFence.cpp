#include "CommandFence.h"

using Citadel::Watchtower::Commands::CommandFence;

CommandFence::CommandFence(handle data) {
	impl = data;
}

CommandFence::CommandFence(const CommandFence& other) {
	this->impl = other.impl;
}

CommandFence::CommandFence(CommandFence&& other) noexcept {
	this->impl = other.impl;
	other.impl = nullptr;
}

CommandFence::~CommandFence() noexcept {
	this->impl;
	this->impl = nullptr;
}

CommandFence&
CommandFence::operator=(const CommandFence& other) {
	if (this != &other) {
		this->impl = other.impl;
	}
	return *this;
}

CommandFence&
CommandFence::operator=(CommandFence&& other) noexcept {
	if (this != &other) {
		this->impl = other.impl;
		other.impl = nullptr;
	}
	return *this;
}

handle
CommandFence::Get() {
	return impl;
}