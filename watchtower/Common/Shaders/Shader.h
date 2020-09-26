#ifndef __SHADER_H__
#define __SHADER_H__

#include <string>

#include "keep/Types.h"
using Citadel::Keep::handle;

namespace Citadel::Watchtower::Shaders {
	class Shader {
	public:
		Shader(handle data = nullptr);
		Shader(const Shader& other);
		Shader(Shader&& other) noexcept;
		~Shader() noexcept;

		Shader& operator=(const Shader& other);
		Shader& operator=(Shader&& other) noexcept;

		handle Get();

	private:;

		handle impl;
	};

}

#endif