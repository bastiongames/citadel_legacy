#include "watchtower/Common/Buffers/VertexAttribute.h"
#include "watchtower/Common/Formats/FormatAttributes.h"

using Citadel::Watchtower::Buffers::VertexAttribute;
using Citadel::Watchtower::Formats::FormatAttributes;

VertexAttribute::VertexAttribute(const char* name, const BufferFormat format, u32 location, u32 instanceDivisor, const SystemValue systemValue):
	name{ name }, format{ format }, location{ location }, systemValue{ systemValue }, instanceDivisor{ instanceDivisor }
{
}

VertexAttribute::VertexAttribute(const char* semanticName, u32 semanticIndex, const BufferFormat format, u32 location, u32 instanceDivisor) :
	name{ semanticName }, format{ format }, location{ location }, semanticIndex{ semanticIndex }, instanceDivisor{ instanceDivisor }
{
}

VertexAttribute::VertexAttribute(const char* name, const BufferFormat format, u32 location, u32 offset, u32 stride, u32 slot, u32 instanceDivisor) :
	VertexAttribute { name, 0, format, location,offset,stride,slot,instanceDivisor }
{
}

VertexAttribute::VertexAttribute(const char* semanticName, u32 semanticIndex, const BufferFormat format, u32 location, u32 offset, u32 stride, u32 slot, u32 instanceDivisor) :
	name{ semanticName }, format{ format }, location{ location }, semanticIndex{ semanticIndex }, slot{ slot }, offset{ offset }, stride{ stride }, instanceDivisor{ instanceDivisor}
{
}

u32
VertexAttribute::GetSize() const {
	const auto& formatAttribs = FormatAttributes::getFormatAttributes(format);
	if((formatAttribs.flags & FormatFlags::SupportsVertex) == FormatFlags::SupportsVertex) {
		return formatAttribs.bitSize / 8;
	}
	return 0;
}