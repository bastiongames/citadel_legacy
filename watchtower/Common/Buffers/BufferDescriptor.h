#ifndef __BUFFERDESCRIPTOR_H__
#define __BUFFERDESCRIPTOR_H__

#include "keep/Types.h"

#include "watchtower/Common/Formats/BufferFormat.h"
#include "watchtower/Common/Flags/BindFlags.h"
#include "watchtower/Common/Flags/CPUAccessFlags.h"
#include "watchtower/Common/Flags/MiscFlags.h"
#include "watchtower/Common/Buffers/VertexAttribute.h"

using Citadel::Keep::u64;
using Citadel::Keep::u32;

using Citadel::Watchtower::Formats::BufferFormat;
using Citadel::Watchtower::Flags::BindFlags;
using Citadel::Watchtower::Flags::CPUAccessFlags;
using Citadel::Watchtower::Flags::MiscFlags;

#include <vector>

namespace Citadel::Watchtower::Buffers {
	struct BufferDescriptor {
		u32 size = 0;
		u32 stride = 0;
		BufferFormat format = BufferFormat::Undefined;
		BindFlags bindFlags = BindFlags::None;
		CPUAccessFlags cpuAccessFlags = CPUAccessFlags::None;
		MiscFlags miscFlags = MiscFlags::None;
		std::vector<VertexAttribute> vertexAttributes;
	};
}

#endif