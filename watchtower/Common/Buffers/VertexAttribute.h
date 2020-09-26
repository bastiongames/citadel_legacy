#ifndef __VERTEXATTRIBUTE_H__
#define __VERTEXATTRIBUTE_H__

#include "keep/Types.h"
using Citadel::Keep::u32;

#include "watchtower/Common/Formats/BufferFormat.h"
using Citadel::Watchtower::Formats::BufferFormat;

#include "watchtower/Common/SystemValue.h"

#include <string>

namespace Citadel::Watchtower::Buffers {
	struct VertexAttribute {
		VertexAttribute() = default;
		VertexAttribute(const VertexAttribute&) = default;
		VertexAttribute& operator =(const VertexAttribute&) = default;

		VertexAttribute(
			const char* name,
			const BufferFormat format,
			u32 location = 0,
			u32 instanceDivisor = 0,
			const SystemValue systemValue = SystemValue::Undefined
		);

		VertexAttribute(
			const char* semanticName,
			u32 semanticIndex,
			const BufferFormat format,
			u32 location = 0,
			u32 instanceDivisor = 0
		);

		VertexAttribute(
			const char* name,
			const BufferFormat format,
			u32 location,
			u32 offset,
			u32 stride,
			u32 slot = 0,
			u32 instanceDivisor = 0
		);

		VertexAttribute(
			const char* semanticName,
			u32 semanticIndex,
			BufferFormat format,
			u32 location,
			u32 offset,
			u32 stride,
			u32 slot = 0,
			u32 instanceDivisor = 0
		);

		u32 GetSize() const;

		std::string name;
		BufferFormat format = BufferFormat::RGBA32Float;
		u32 location = 0;
		u32 semanticIndex = 0;
		SystemValue systemValue = SystemValue::Undefined;
		u32 slot = 0;
		u32 offset = 0;
		u32 stride = 0;
		u32 instanceDivisor = 0;
	};

	bool operator == (const VertexAttribute& lhs, const VertexAttribute& rhs);
	bool operator != (const VertexAttribute& lhs, const VertexAttribute& rhs);
}

#endif