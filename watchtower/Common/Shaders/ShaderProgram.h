#ifndef __SHADERPROGRAM_H__
#define __SHADERPROGRAM_H__

#include <string>

#include "keep/Types.h"
using Citadel::Keep::handle;

namespace Citadel::Watchtower::Shaders {
	class ShaderProgram {
	public:
		ShaderProgram(handle data = nullptr);
		ShaderProgram(const ShaderProgram& other);
		ShaderProgram(ShaderProgram&& other) noexcept;
		~ShaderProgram() noexcept;

		ShaderProgram& operator=(const ShaderProgram& other);
		ShaderProgram& operator=(ShaderProgram&& other) noexcept;

		handle Get();

	private:;

		   handle impl;
	};

}

#endif