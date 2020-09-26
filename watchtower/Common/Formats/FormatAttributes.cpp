#include "FormatAttributes.h"
using namespace Citadel::Keep;

using Citadel::Watchtower::Formats::FormatAttributes;

using Citadel::Watchtower::Formats::ImageFormat;
using Citadel::Watchtower::Formats::DataType;
using Citadel::Watchtower::Flags::FormatFlags;

static const FormatFlags GenerateMips = FormatFlags::SupportsGenerateMips | FormatFlags::SupportsMips | FormatFlags::SupportsRenderTarget;
static const FormatFlags Tex123D = FormatFlags::SupportsTexture1D | FormatFlags::SupportsTexture2D | FormatFlags::SupportsTexture3D;
static const FormatFlags AllTex = Tex123D | FormatFlags::SupportsTextureCube;
static const FormatFlags UInt = FormatFlags::IsUnsigned | FormatFlags::IsInteger;
static const FormatFlags UNorm = UInt | FormatFlags::IsNormalized;
static const FormatFlags SInt = FormatFlags::IsInteger;
static const FormatFlags SNorm = SInt | FormatFlags::IsNormalized;

static const FormatAttributes formatAttribs[] = {
	{ 0, 0, 0, 0, ImageFormat::R, DataType::Undefined, FormatFlags::None },
	
	{ 8, 1, 1, 1, ImageFormat::Alpha, DataType::UInt8, GenerateMips | AllTex | UNorm },
	
	{ 8, 1, 1, 1, ImageFormat::R, DataType::UInt8, FormatFlags::SupportsVertex | GenerateMips | AllTex | UNorm },
	{ 8, 1, 1, 1, ImageFormat::R, DataType::Int8, FormatFlags::SupportsVertex | GenerateMips | AllTex | UNorm },
	{ 8, 1, 1, 1, ImageFormat::R, DataType::UInt8, FormatFlags::SupportsVertex | GenerateMips | AllTex | UInt },
	{ 8, 1, 1, 1, ImageFormat::R, DataType::Int8, FormatFlags::SupportsVertex | GenerateMips | AllTex | SInt },

	{ 16, 1, 1, 1, ImageFormat::R, DataType::UInt16, FormatFlags::SupportsVertex | GenerateMips | AllTex | UNorm },
	{ 16, 1, 1, 1, ImageFormat::R, DataType::Int16, FormatFlags::SupportsVertex | GenerateMips | AllTex | SNorm },
	{ 16, 1, 1, 1, ImageFormat::R, DataType::UInt16, FormatFlags::SupportsVertex | GenerateMips | AllTex | UInt },
	{ 16, 1, 1, 1, ImageFormat::R, DataType::Int16, FormatFlags::SupportsVertex | GenerateMips | AllTex | SInt },
	{ 16, 1, 1, 1, ImageFormat::R, DataType::Float16, FormatFlags::SupportsVertex | GenerateMips | AllTex | FormatFlags::IsUnsigned },

	{ 32, 1, 1, 1, ImageFormat::R, DataType::UInt32, FormatFlags::SupportsVertex | GenerateMips | AllTex | UInt },
	{ 32, 1, 1, 1, ImageFormat::R, DataType::Int32, FormatFlags::SupportsVertex | GenerateMips | AllTex | SInt },
	{ 32, 1, 1, 1, ImageFormat::R, DataType::Float32, FormatFlags::SupportsVertex | GenerateMips | AllTex | FormatFlags::IsUnsigned },

	{ 64, 1, 1, 1, ImageFormat::R, DataType::Float64, FormatFlags::SupportsVertex | GenerateMips | AllTex | FormatFlags::IsUnsigned },

	{ 16, 1, 1, 2, ImageFormat::RG, DataType::UInt8, FormatFlags::SupportsVertex | GenerateMips | AllTex | UNorm },
	{ 16, 1, 1, 2, ImageFormat::RG, DataType::Int8, FormatFlags::SupportsVertex | GenerateMips | AllTex | SNorm },
	{ 16, 1, 1, 2, ImageFormat::RG, DataType::UInt8, FormatFlags::SupportsVertex | GenerateMips | AllTex | UInt },
	{ 16, 1, 1, 2, ImageFormat::RG, DataType::Int8, FormatFlags::SupportsVertex | GenerateMips | AllTex | SInt },

	{ 32, 1, 1, 2, ImageFormat::RG, DataType::UInt16, FormatFlags::SupportsVertex | GenerateMips | AllTex | UNorm },
	{ 32, 1, 1, 2, ImageFormat::RG, DataType::Int16, FormatFlags::SupportsVertex | GenerateMips | AllTex | SNorm },
	{ 32, 1, 1, 2, ImageFormat::RG, DataType::UInt16, FormatFlags::SupportsVertex | GenerateMips | AllTex | UInt },
	{ 32, 1, 1, 2, ImageFormat::RG, DataType::Int16, FormatFlags::SupportsVertex | GenerateMips | AllTex | SInt },
	{ 32, 1, 1, 2, ImageFormat::RG, DataType::Float16, FormatFlags::SupportsVertex | GenerateMips | AllTex | FormatFlags::IsUnsigned },

	{ 64, 1, 1, 2, ImageFormat::RG, DataType::UInt32, FormatFlags::SupportsVertex | GenerateMips | AllTex | UInt },
	{ 64, 1, 1, 2, ImageFormat::RG, DataType::Int32, FormatFlags::SupportsVertex | GenerateMips | AllTex | SInt },
	{ 64, 1, 1, 2, ImageFormat::RG, DataType::Float32, FormatFlags::SupportsVertex | GenerateMips | AllTex | FormatFlags::IsUnsigned },

	{ 128, 1, 1, 2, ImageFormat::RG, DataType::Float64, FormatFlags::SupportsVertex | GenerateMips | AllTex | FormatFlags::IsUnsigned },

	{ 24, 1, 1, 3, ImageFormat::RGB, DataType::UInt8, FormatFlags::SupportsVertex | GenerateMips | AllTex | UNorm },
	{ 24, 1, 1, 3, ImageFormat::RGB, DataType::UInt8, FormatFlags::SupportsVertex | GenerateMips | AllTex | UNorm | FormatFlags::IsColorSpace_sRGB },
	{ 24, 1, 1, 3, ImageFormat::RGB, DataType::Int8, FormatFlags::SupportsVertex | GenerateMips | AllTex | SNorm },
	{ 24, 1, 1, 3, ImageFormat::RGB, DataType::UInt8, FormatFlags::SupportsVertex | GenerateMips | AllTex | UInt },
	{ 24, 1, 1, 3, ImageFormat::RGB, DataType::Int8, FormatFlags::SupportsVertex | GenerateMips | AllTex | SInt },

	{ 48, 1, 1, 3, ImageFormat::RGB, DataType::UInt16, FormatFlags::SupportsVertex | GenerateMips | AllTex | UNorm },
	{ 48, 1, 1, 3, ImageFormat::RGB, DataType::Int16, FormatFlags::SupportsVertex | GenerateMips | AllTex | SNorm },
	{ 48, 1, 1, 3, ImageFormat::RGB, DataType::UInt16, FormatFlags::SupportsVertex | GenerateMips | AllTex | UInt},
	{ 48, 1, 1, 3, ImageFormat::RGB, DataType::Int16, FormatFlags::SupportsVertex | GenerateMips | AllTex | SInt },
	{ 48, 1, 1, 3, ImageFormat::RGB, DataType::Float16, FormatFlags::SupportsVertex | GenerateMips | AllTex | FormatFlags::IsUnsigned },

	{ 96, 1, 1, 3, ImageFormat::RGB, DataType::UInt32, FormatFlags::SupportsVertex | GenerateMips | AllTex | UInt },
	{ 96, 1, 1, 3, ImageFormat::RGB, DataType::Int32, FormatFlags::SupportsVertex | GenerateMips | AllTex | SInt },
	{ 96, 1, 1, 3, ImageFormat::RGB, DataType::Float32, FormatFlags::SupportsVertex | GenerateMips | AllTex | FormatFlags::IsUnsigned },

	{ 192, 1, 1, 3, ImageFormat::RGB, DataType::Float64, FormatFlags::SupportsVertex | GenerateMips | AllTex | FormatFlags::IsUnsigned },

	{ 32, 1, 1, 4, ImageFormat::RGBA, DataType::UInt8, FormatFlags::SupportsVertex | GenerateMips | AllTex | UNorm },
	{ 32, 1, 1, 4, ImageFormat::RGBA, DataType::UInt8, FormatFlags::SupportsVertex | GenerateMips | AllTex | UNorm | FormatFlags::IsColorSpace_sRGB },
	{ 32, 1, 1, 4, ImageFormat::RGBA, DataType::Int8, FormatFlags::SupportsVertex | GenerateMips | AllTex | SNorm },
	{ 32, 1, 1, 4, ImageFormat::RGBA, DataType::UInt8, FormatFlags::SupportsVertex | GenerateMips | AllTex | UInt },
	{ 32, 1, 1, 4, ImageFormat::RGBA, DataType::Int8, FormatFlags::SupportsVertex | GenerateMips | AllTex | SInt },

	{ 64, 1, 1, 4, ImageFormat::RGBA, DataType::UInt16, FormatFlags::SupportsVertex | GenerateMips | AllTex | UNorm },
	{ 64, 1, 1, 4, ImageFormat::RGBA, DataType::Int16, FormatFlags::SupportsVertex | GenerateMips | AllTex | UNorm },
	{ 64, 1, 1, 4, ImageFormat::RGBA, DataType::UInt16, FormatFlags::SupportsVertex | GenerateMips | AllTex | UNorm },
	{ 64, 1, 1, 4, ImageFormat::RGBA, DataType::Int16, FormatFlags::SupportsVertex | GenerateMips | AllTex | UNorm },
	{ 64, 1, 1, 4, ImageFormat::RGBA, DataType::Float16, FormatFlags::SupportsVertex | GenerateMips | AllTex | FormatFlags::IsUnsigned },
	
	{ 128, 1, 1, 4, ImageFormat::RGBA, DataType::UInt32, FormatFlags::SupportsVertex | GenerateMips | AllTex | UInt },
	{ 128, 1, 1, 4, ImageFormat::RGBA, DataType::Int32, FormatFlags::SupportsVertex | GenerateMips | AllTex | SInt },
	{ 128, 1, 1, 4, ImageFormat::RGBA, DataType::Float32, FormatFlags::SupportsVertex | GenerateMips | AllTex | FormatFlags::IsUnsigned },

	{ 256, 1, 1, 4, ImageFormat::RGBA, DataType::Float64, FormatFlags::SupportsVertex | GenerateMips | AllTex | FormatFlags::IsUnsigned },

	{ 32, 1, 1, 4, ImageFormat::BGRA, DataType::UInt8, GenerateMips | AllTex | UNorm },
	{ 32, 1, 1, 4, ImageFormat::BGRA, DataType::UInt8, GenerateMips | AllTex | UNorm | FormatFlags::IsColorSpace_sRGB },
	{ 32, 1, 1, 4, ImageFormat::BGRA, DataType::Int8, GenerateMips | AllTex | SNorm },
	{ 32, 1, 1, 4, ImageFormat::BGRA, DataType::UInt8, GenerateMips | AllTex | UInt },
	{ 32, 1, 1, 4, ImageFormat::BGRA, DataType::Int8, GenerateMips | AllTex | SInt },

	{ 32, 1, 1, 4, ImageFormat::RGBA, DataType::Undefined, GenerateMips | AllTex | UNorm | FormatFlags::IsPacked },
	{ 32, 1, 1, 4, ImageFormat::RGBA, DataType::Undefined, GenerateMips | AllTex | UInt | FormatFlags::IsPacked },
	{ 32, 1, 1, 3, ImageFormat::RGB, DataType::Undefined, GenerateMips | AllTex | FormatFlags::IsUnsigned | FormatFlags::IsPacked },
	{ 32, 1, 1, 3, ImageFormat::RGB, DataType::Undefined, GenerateMips | AllTex | FormatFlags::IsUnsigned | FormatFlags::IsPacked },

	{ 16, 1, 1, 1, ImageFormat::Depth, DataType::UInt16, FormatFlags::SupportsMips | FormatFlags::SupportsRenderTarget | AllTex | FormatFlags::HasDepth },
	{ 32, 1, 1, 2, ImageFormat::DepthStencil, DataType::UInt16, FormatFlags::SupportsMips | FormatFlags::SupportsRenderTarget | AllTex | FormatFlags::HasDepthStencil},
	{ 32, 1, 1, 1, ImageFormat::Depth, DataType::Float32, FormatFlags::SupportsMips | FormatFlags::SupportsRenderTarget | AllTex | FormatFlags::HasDepth },
	{ 64, 1, 1, 2, ImageFormat::DepthStencil, DataType::Float32, FormatFlags::SupportsMips | FormatFlags::SupportsRenderTarget | AllTex | FormatFlags::HasDepthStencil },

	{ 64, 4, 4, 4, ImageFormat::BC1, DataType::UInt8, FormatFlags::SupportsMips | FormatFlags::SupportsTexture2D | FormatFlags::SupportsTexture3D | FormatFlags::SupportsTextureCube | FormatFlags::IsCompressed | FormatFlags::IsUnsignedInteger | FormatFlags::IsNormalized },
	{ 64, 4, 4, 4, ImageFormat::BC1, DataType::UInt8, FormatFlags::SupportsMips | FormatFlags::SupportsTexture2D | FormatFlags::SupportsTexture3D | FormatFlags::SupportsTextureCube | FormatFlags::IsCompressed | FormatFlags::IsUnsignedInteger | FormatFlags::IsNormalized | FormatFlags::IsColorSpace_sRGB },
	{ 128, 4, 4, 4, ImageFormat::BC2, DataType::UInt8, FormatFlags::SupportsMips | FormatFlags::SupportsTexture2D | FormatFlags::SupportsTexture3D | FormatFlags::SupportsTextureCube | FormatFlags::IsCompressed | FormatFlags::IsUnsignedInteger | FormatFlags::IsNormalized },
	{ 128, 4, 4, 4, ImageFormat::BC2, DataType::UInt8, FormatFlags::SupportsMips | FormatFlags::SupportsTexture2D | FormatFlags::SupportsTexture3D | FormatFlags::SupportsTextureCube | FormatFlags::IsCompressed | FormatFlags::IsUnsignedInteger | FormatFlags::IsNormalized | FormatFlags::IsColorSpace_sRGB },
	{ 128, 4, 4, 4, ImageFormat::BC3, DataType::UInt8, FormatFlags::SupportsMips | FormatFlags::SupportsTexture2D | FormatFlags::SupportsTexture3D | FormatFlags::SupportsTextureCube | FormatFlags::IsCompressed | FormatFlags::IsUnsignedInteger | FormatFlags::IsNormalized },
	{ 128, 4, 4, 4, ImageFormat::BC3, DataType::UInt8, FormatFlags::SupportsMips | FormatFlags::SupportsTexture2D | FormatFlags::SupportsTexture3D | FormatFlags::SupportsTextureCube | FormatFlags::IsCompressed | FormatFlags::IsUnsignedInteger | FormatFlags::IsNormalized | FormatFlags::IsColorSpace_sRGB },
	{ 64, 4, 4, 4, ImageFormat::BC4, DataType::UInt8, FormatFlags::SupportsMips | FormatFlags::SupportsTexture2D | FormatFlags::SupportsTexture3D | FormatFlags::SupportsTextureCube | FormatFlags::IsCompressed | FormatFlags::IsUnsignedInteger | FormatFlags::IsNormalized },
	{ 64, 4, 4, 4, ImageFormat::BC4, DataType::Int8, FormatFlags::SupportsMips | FormatFlags::SupportsTexture2D | FormatFlags::SupportsTexture3D | FormatFlags::SupportsTextureCube | FormatFlags::IsCompressed | FormatFlags::IsInteger | FormatFlags::IsNormalized },
	{ 128, 4, 4, 4, ImageFormat::BC5, DataType::UInt8, FormatFlags::SupportsMips | FormatFlags::SupportsTexture2D | FormatFlags::SupportsTexture3D | FormatFlags::SupportsTextureCube | FormatFlags::IsCompressed | FormatFlags::IsUnsignedInteger | FormatFlags::IsNormalized },
	{ 128, 4, 4, 4, ImageFormat::BC5, DataType::Int8, FormatFlags::SupportsMips | FormatFlags::SupportsTexture2D | FormatFlags::SupportsTexture3D | FormatFlags::SupportsTextureCube | FormatFlags::IsCompressed | FormatFlags::IsInteger | FormatFlags::IsNormalized },
};

FormatAttributes
FormatAttributes::getFormatAttributes(const BufferFormat format) {
	auto index = static_cast<u32>(format);
	if (index < (_countof(formatAttribs))) {
		return formatAttribs[index];
	}
	return formatAttribs[0];
}