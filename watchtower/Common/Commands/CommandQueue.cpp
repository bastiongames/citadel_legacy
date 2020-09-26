#include "CommandQueue.h"

using Citadel::Watchtower::Commands::CommandQueue;

CommandQueue::CommandQueue(handle data) {
	impl = data;
}

CommandQueue::CommandQueue(const CommandQueue& other) {
	this->impl = other.impl;
}

CommandQueue::CommandQueue(CommandQueue&& other) noexcept {
	this->impl = other.impl;
	other.impl = nullptr;
}

CommandQueue::~CommandQueue() noexcept {
	this->impl;
	this->impl = nullptr;
}

CommandQueue&
CommandQueue::operator=(const CommandQueue& other) {
	if (this != &other) {
		this->impl = other.impl;
	}
	return *this;
}

CommandQueue&
CommandQueue::operator=(CommandQueue&& other) noexcept {
	if (this != &other) {
		this->impl = other.impl;
		other.impl = nullptr;
	}
	return *this;
}

handle
CommandQueue::Get() {
	return impl;
}