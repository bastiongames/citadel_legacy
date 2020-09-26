#ifndef __SHADERPROGRAM_H__
#define __SHADERPROGRAM_H__

#include "Shader.h"
#include "ShaderType.h"

namespace Citadel::Watchtower::Shaders {
	class ShaderProgram {
	public:
		virtual void* GetBytecode(ShaderType type) = 0;
	};
	using PShaderProgram = Keep::SPtr<ShaderProgram>;
}

#endif