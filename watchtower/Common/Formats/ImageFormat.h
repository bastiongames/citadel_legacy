#ifndef __IMAGEFORMAT_H__
#define __IMAGEFORMAT_H__

namespace Citadel::Watchtower::Formats {
	enum struct ImageFormat {
		// Color
		Alpha,
		R,
		RG,
		RGB,
		BGR,
		RGBA,
		BGRA,
		ARGB,
		ABGR,

		// Depth-Stencil
		Depth,
		DepthStencil,

		// Compressed
		BC1,
		BC2,
		BC3,
		BC4,
		BC5,
	};
}

#endif