#ifndef __DEVICE_H__
#define __DEVICE_H__

#include "keep/Types.h"
using Citadel::Keep::handle;

namespace Citadel::Watchtower {
	class Device {
	public:
		Device(handle data = nullptr);
		Device(const Device& context);
		Device(Device&& context) noexcept;
		~Device() noexcept;

		Device& operator=(const Device& context);
		Device& operator=(Device&& context) noexcept;

		handle Get() { return impl; }

		handle AcquireDevice(handle platformData);

	private:
		handle impl;
	};
}

#endif