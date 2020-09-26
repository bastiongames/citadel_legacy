#ifndef __FORMATFLAGS_H__
#define __FORMATFLAGS_H__

namespace Citadel::Watchtower::Flags {
	enum struct FormatFlags {
		None = 0,

		HasDepth = (1 << 0),
		HasStencil = (1 << 1),
		IsColorSpace_sRGB = (1 << 2),
		IsCompressed = (1 << 3),
		IsNormalized = (1 << 4),
		IsInteger = (1 << 5),
		IsUnsigned = (1 << 6),
		IsPacked = (1 << 7),
		SupportsRenderTarget = (1 << 8),
		SupportsMips = (1 << 9),
		SupportsGenerateMips = (1 << 10),
		SupportsTexture1D = (1 << 11),
		SupportsTexture2D = (1 << 12),
		SupportsTexture3D = (1 << 13),
		SupportsTextureCube = (1 << 14),
		SupportsVertex = (1 << 15),
		IsUnsignedInteger = (IsUnsigned | IsInteger),
		HasDepthStencil = (HasDepth | HasStencil),
	};

	FormatFlags operator | (FormatFlags lhs, FormatFlags rhs);
	FormatFlags operator & (FormatFlags lhs, FormatFlags rhs);
}

#endif