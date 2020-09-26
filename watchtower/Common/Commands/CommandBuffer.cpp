#include "CommandBuffer.h"

using Citadel::Watchtower::Commands::CommandBuffer;

CommandBuffer::CommandBuffer(handle data) {
	impl = data;
}

CommandBuffer::CommandBuffer(const CommandBuffer& other) {
	this->impl = other.impl;
}

CommandBuffer::CommandBuffer(CommandBuffer&& other) noexcept {
	this->impl = other.impl;
	other.impl = nullptr;
}

CommandBuffer::~CommandBuffer() noexcept {
	this->impl;
	this->impl = nullptr;
}

CommandBuffer&
CommandBuffer::operator=(const CommandBuffer& other) {
	if (this != &other) {
		this->impl = other.impl;
	}
	return *this;
}

CommandBuffer&
CommandBuffer::operator=(CommandBuffer&& other) noexcept {
	if (this != &other) {
		this->impl = other.impl;
		other.impl = nullptr;
	}
	return *this;
}

handle
CommandBuffer::Get() {
	return impl;
}