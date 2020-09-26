#ifndef __VERTEXFORMAT_H__
#define __VERTEXFORMAT_H__

#include "keep/Types.h"
#include "watchtower/Common/Buffers/VertexAttribute.h"

#include <vector>

using Citadel::Keep::u32;

namespace Citadel::Watchtower::Buffers {
	struct VertexFormat {
		void AppendAttribute(const VertexAttribute& attrib, bool customLocation = false, u32 customOffset = ~0);
		u32 GetStride() const;
		u32 GetStride(u32 slot) const;
		void SetStride(u32 stride);
		void SetStride(u32 stride, u32 slot);
		void SetSlot(u32 slot);
		
		std::vector<VertexAttribute> attributes;
	};
}

#endif