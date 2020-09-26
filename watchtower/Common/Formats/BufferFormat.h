#ifndef __FORMAT_H__
#define __FORMAT_H__

namespace Citadel::Watchtower::Formats {
	enum struct BufferFormat {
		Undefined,

		// Alpha channel formats
		A8UNorm,

		// Red channel formats
		R8UNorm,
		R8SNorm,
		R8UInt,
		R8SInt,

		R16UNorm,
		R16SNorm,
		R16UInt,
		R16SInt,
		R16Float,

		R32UInt,
		R32SInt,
		R32Float,

		R64Float,

		// Red-Green channel formats
		RG8UNorm,
		RG8SNorm,
		RG8UInt,
		RG8SInt,

		RG16UNorm,
		RG16SNorm,
		RG16UInt,
		RG16SInt,
		RG16Float,

		RG32UInt,
		RG32SInt,
		RG32Float,

		RG64Float,

		// Red-Green-Blue channel formats
		RGB8UNorm,
		RGB8SNorm,
		RGB8UInt,
		RGB8SInt,

		RGB16UNorm,
		RGB16SNorm,
		RGB16UInt,
		RGB16SInt,
		RGB16Float,

		RGB32UInt,
		RGB32SInt,
		RGB32Float,

		RGB64Float,

		// Red-Green-Blue-Alpha channel formats
		RGBA8UNorm,
		RGBA8UNorm_sRGB,
		RGBA8SNorm,
		RGBA8UInt,
		RGBA8SInt,

		RGBA16UNorm,
		RGBA16SNorm,
		RGBA16UInt,
		RGBA16SInt,
		RGBA16Float,

		RGBA32UInt,
		RGBA32SInt,
		RGBA32Float,

		RGBA64Float,

		// Blue-Green-Red-Alpha channel formats
		BGRA8UNorm,
		BGRA8UNorm_sRGB,
		BGRA8SNorm,
		BGRA8UInt,
		BGRA8SInt,

		// Packed formats
		RGB10A2UNorm,
		RGB10A2UInt,
		RG11B10Float,
		RGB9E5Float,

		// Depth-Stencil formats
		D16UNorm,
		D24UNormS8UInt,
		D32Float,
		D32FloatS8X24UInt,

		// Block Compression formats
		BC1Unorm,
		BC1UNorm_sRGB,
		BC2Unorm,
		BC2UNorm_sRGB,
		BC3Unorm,
		BC3UNorm_sRGB,
		BC4UNorm,
		BC5UNorm,

	};
}

#endif