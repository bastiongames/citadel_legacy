#ifndef __SHADERPROGRAMDESCRIPTOR_H__
#define __SHADERPROGRAMDESCRIPTOR_H__

#include "Shader.h"

namespace Citadel::Watchtower::Shaders {
	struct ShaderProgramDescriptor {
		PShader vertexShader = nullptr;
		PShader hullShader = nullptr;
		PShader domainShader = nullptr;
		PShader geometryShader = nullptr;
		PShader pixelShader = nullptr;
		PShader computeShader = nullptr;
	};
}

#endif