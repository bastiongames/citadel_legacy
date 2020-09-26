#include "Device.h"

using Citadel::Watchtower::Device;

Device::Device(handle data) {
	impl = AcquireDevice(data);
}

Device::Device(const Device& context) {
	impl = context.impl;
}

Device::Device(Device&& context) noexcept {
	this->impl = context.impl;
	context.impl = nullptr;
}

Device::~Device() noexcept {
	this->impl = nullptr;
}

Device&
Device::operator=(const Device& context) {
	if (this != &context) {
		this->impl = context.impl;
	}
	return *this;
}

Device&
Device::operator=(Device&& context) noexcept {
	if (this != &context) {
		this->impl = context.impl;
		context.impl = nullptr;
	}
	return *this;
}