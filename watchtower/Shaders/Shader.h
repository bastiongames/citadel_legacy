#ifndef __SHADER_H__
#define __SHADER_H__

#include "keep/Types.h"

namespace Citadel::Watchtower::Shaders {
	class Shader {
	public:
		virtual void* GetBytecode() = 0;
	private:
	};

	using PShader = Citadel::Keep::SPtr<Shader>;
}

#endif