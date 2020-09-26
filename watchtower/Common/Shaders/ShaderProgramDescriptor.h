#ifndef __SHADERPROGRAMDESCRIPTOR_H__
#define __SHADERPROGRAMDESCRIPTOR_H__

#include "keep/Types.h"
#include "Shader.h"

using Citadel::Keep::SPtr;

namespace Citadel::Watchtower::Shaders {
	

	struct ShaderProgramDescriptor {
		SPtr<Shader> vertexShader = nullptr;
		SPtr<Shader> hullShader = nullptr;
		SPtr<Shader> domainShader = nullptr;
		SPtr<Shader> geometryShader = nullptr;
		SPtr<Shader> pixelShader = nullptr;
		SPtr<Shader> computeShader = nullptr;
	};
}

#endif