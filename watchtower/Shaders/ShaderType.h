#ifndef __SHADERTYPE_H__
#define __SHADERTYPE_H__

namespace Citadel::Watchtower::Shaders {
	enum struct ShaderType {
		Undefined,
		Vertex,
		HullShader,
		DomainShader,
		Geometry,
		Pixel,
		Compute,

		TessControl = HullShader,
		TessEvaluation = DomainShader,
		Fragment = Pixel,
	};
}

#endif