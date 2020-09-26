#ifndef __SWAPCHAINDESCRIPTOR_H__
#define __SWAPCHAINDESCRIPTOR_H__


namespace Citadel::Watchtower {
	struct SwapchainDescriptor {
		Keep::u32 width;
		Keep::u32 height;
		Keep::u32 frameCount;
	};
}

#endif