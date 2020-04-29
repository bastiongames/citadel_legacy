#include "VertexBuffer.h"

Citadel::Watchtower::VertexBuffer::VertexBuffer(handle impl) {
	this->impl = impl;
}

Citadel::Watchtower::VertexBuffer::VertexBuffer(const VertexBuffer& other) {
	this->impl = other.impl;
}

Citadel::Watchtower::VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept{
	this->impl = other.impl;
	other.impl = nullptr;
}

Citadel::Watchtower::VertexBuffer::~VertexBuffer() noexcept {
	this->impl = nullptr;
}

Citadel::Watchtower::VertexBuffer&
Citadel::Watchtower::VertexBuffer::operator=(const VertexBuffer& other) {
	if (this != &other) {
		this->impl = other.impl;
	}

	return *this;
}

Citadel::Watchtower::VertexBuffer&
Citadel::Watchtower::VertexBuffer::operator=(VertexBuffer&& other) noexcept {
	if (this != &other) {
		this->impl = other.impl;
		other.impl = nullptr;
	}

	return *this;
}