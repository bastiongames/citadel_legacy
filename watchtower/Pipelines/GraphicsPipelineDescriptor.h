#ifndef __GRAPHICSPIPELINEDESCRIPTOR_H__
#define __GRAPHICSPIPELINEDESCRIPTOR_H__

#include "watchtower/Shaders/ShaderProgram.h"
#include "watchtower/Pipelines/PipelineLayout.h"
#include "watchtower/RenderPass.h"

namespace Citadel::Watchtower::Pipelines {
	struct GraphicsPipelineDescriptor {
		Shaders::PShaderProgram shaderProgram;
		Pipelines::PPipelineLayout pipelineLayout;
		PRenderPass renderPass;
	};
}

#endif