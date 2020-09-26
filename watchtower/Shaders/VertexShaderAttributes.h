#ifndef __VERTEXSHADERATTRIBUTES_H__
#define __VERTEXSHADERATTRIBUTES_H__

#include <vector>

#include "watchtower/Common/Buffers/VertexAttribute.h"

using Citadel::Watchtower::Buffers::VertexAttribute;

namespace Citadel::Watchtower::Shaders {
	struct VertexShaderAttributes {
		std::vector<VertexAttribute> input;
		std::vector<VertexAttribute> output;
	};
}

#endif
