#ifndef __FORMATATTRIBUTES_H__
#define __FORMATATTRIBUTES_H__

#include "keep/Types.h"

using Citadel::Keep::u16;
using Citadel::Keep::u8;

#include "watchtower/Common/Formats/BufferFormat.h"
#include "watchtower/Common/Formats/ImageFormat.h"
#include "watchtower/Common/Formats/DataType.h"

#include "watchtower/Common/Flags/FormatFlags.h"
using Citadel::Watchtower::Flags::FormatFlags;

namespace Citadel::Watchtower::Formats {
	struct FormatAttributes {
		u16 bitSize;
		u8 blockWidth;
		u8 blockHeight;
		u8 components;
		ImageFormat format;
		DataType dataType;
		FormatFlags flags;

		static FormatAttributes getFormatAttributes(const BufferFormat format);
	};
}

#endif