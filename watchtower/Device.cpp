#include "Device.h"

Citadel::Watchtower::Device::Device() {
	impl = nullptr;
}

Citadel::Watchtower::Device::Device(handle data) {
	this->impl = AcquireDevice(data);
}

Citadel::Watchtower::Device::Device(const Device& other) {
	this->impl = other.impl;
}

Citadel::Watchtower::Device::Device(Device&& other) noexcept {
	this->impl = other.impl;
	other.impl = nullptr;
}

Citadel::Watchtower::Device::~Device() {
	ReleaseDevice();
	
	this->impl = nullptr;
}

Citadel::Watchtower::Device&
Citadel::Watchtower::Device::operator=(const Device& other) {
	if (this != &other) {
		this->impl = other.impl;
	}
	return *this;
}

Citadel::Watchtower::Device&
Citadel::Watchtower::Device::operator=(Device&& other) noexcept {
	if (this != &other) {
		this->impl = other.impl;
		other.impl = nullptr;
	}
	return *this;
}