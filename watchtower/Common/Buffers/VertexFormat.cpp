#include "watchtower/Common/Buffers/VertexFormat.h"

using Citadel::Watchtower::Buffers::VertexAttribute;
using Citadel::Watchtower::Buffers::VertexFormat;

static void 
UpdateStride(VertexFormat& vertexFormat) {
	u32 stride = 0;
	for(const auto& attr : vertexFormat.attributes) {
		stride = std::max(stride, attr.offset + attr.GetSize());
	}
	vertexFormat.SetStride(stride);
}

void
VertexFormat::AppendAttribute(const VertexAttribute& attrib, bool customLocation, u32 customOffset) {
	attributes.push_back(attrib);
	auto& attr = attributes.back();

	if(!customLocation && attr.location == 0) {
		if (attributes.size() > 1) {
			const auto& prevAttr = attributes[attributes.size() - 2];
			attr.location = prevAttr.location + 1;
		}
		else {
			attr.location = 0;
		}
	}

	if (customOffset == ~0) {
		if (attributes.size() > 1) {
			const auto& prevAttr = attributes[attributes.size() - 2];
			attr.offset = prevAttr.offset + prevAttr.GetSize();
		}
		else {
			attr.offset= 0;
		}
	}
	else {
		attr.offset = customOffset;
	}
	UpdateStride(*this);
}

u32
VertexFormat::GetStride() const {
	if (attributes.empty()) {
		return 0;
	}
	return attributes.front().stride;
}

u32
VertexFormat::GetStride(u32 slot) const {
	for (const auto& attr : attributes) {
		if (attr.slot == slot) {
			return attr.stride;
		}
	}
	return 0;
}

void
VertexFormat::SetStride(u32 stride) {
	for (auto& attr : attributes) {
		attr.stride = stride;
	}
}

void
VertexFormat::SetStride(u32 stride, u32 slot) {
	for (auto& attr : attributes) {
		if (attr.slot == slot) {
			attr.stride = stride;
		}
	}
}

void
VertexFormat::SetSlot(u32 slot) {
	for (auto& attr : attributes) {
		attr.slot = slot;
	}
}