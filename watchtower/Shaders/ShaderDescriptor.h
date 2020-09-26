#ifndef __SHADERDESCRIPTOR_H__
#define __SHADERDESCRIPTOR_H__

#include "ShaderType.h"
#include "VertexShaderAttributes.h"

namespace Citadel::Watchtower::Shaders {
	struct ShaderDescriptor {
		ShaderDescriptor() = default;
		ShaderDescriptor(const ShaderDescriptor&) = default;
		ShaderDescriptor& operator= (const ShaderDescriptor&) = default;

		inline ShaderDescriptor(const ShaderType type, const char* source) :
			type{ type }, source{ source } { }
		inline ShaderDescriptor(const ShaderType type, const char* source, const char* entryPoint, const char* profile, u32 flags = 0) :
			type{ type }, source{ source }, entryPoint{ entryPoint }, profile{ profile }, flags{ flags } { }

		ShaderType type = ShaderType::Undefined;
		const char* source = nullptr;
		u32 sourceSize = 0;
		//ShaderSourceType sourceType = ShaderSourceType::CodeFile;
		const char* entryPoint = nullptr;
		const char* profile = nullptr;
		u32 flags = 0;
		
		VertexShaderAttributes vertex;
		//PixelShaderAttributes pixel;
		//ComputeShaderAttributes compute;
	};
}

#endif