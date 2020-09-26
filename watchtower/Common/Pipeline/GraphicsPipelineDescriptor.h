#ifndef __GRAPHICSPIPELINEDESCRIPTOR_H__
#define __GRAPHICSPIPELINEDESCRIPTOR_H__

#include "watchtower/Common/Shaders/ShaderProgram.h"

#include "keep/Types.h"
using Citadel::Keep::SPtr;

namespace Citadel::Watchtower::Pipeline {
	struct GraphicsPipelineDescriptor {
		SPtr<Shaders::ShaderProgram> shaderProgram;
	};
}

#endif