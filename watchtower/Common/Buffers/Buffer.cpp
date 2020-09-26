#include "Buffer.h"

using Citadel::Watchtower::Buffers::Buffer;

Buffer::Buffer(handle data) {
	impl = data;
}

Buffer::Buffer(const Buffer& buffer) {
	impl = buffer.impl;
}

Buffer::Buffer(Buffer&& buffer) noexcept {
	this->impl = buffer.impl;
	buffer.impl = nullptr;
}

Buffer::~Buffer() noexcept {
	this->impl = nullptr;
}

Buffer&
Buffer::operator=(const Buffer& buffer) {
	if (this != &buffer) {
		this->impl = buffer.impl;
	}

	return *this;
}

Buffer&
Buffer::operator=(Buffer&& buffer) noexcept {
	if (this != &buffer) {
		this->impl = buffer.impl;
		buffer.impl = nullptr;
	}
	return *this;
}