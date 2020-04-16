#ifndef __SHADER_H__
#define __SHADER_H__

#include <string>

#include "keep/Types.h"
using Citadel::Keep::handle;

namespace Citadel::Watchtower {
	class Shader {
	public:
		Shader();
		Shader(std::string text, std::string entrypoint, std::string target);
		Shader(const Shader& other);
		Shader(Shader&& other) noexcept;
		~Shader() noexcept;

		Shader& operator=(const Shader& other);
		Shader& operator=(Shader&& other) noexcept;

		handle Get();

	private:
		handle Compile(std::string text, std::string entrypoint, std::string target);

		handle impl;
	};

	class VertexShader : public Shader {
	public:
		VertexShader() : Shader() { }
		VertexShader(std::string text, std::string entrypoint): Shader(text, entrypoint, "vs_5_0") { }
	};

	class PixelShader : public Shader {
	public:
		PixelShader(): Shader() { }
		PixelShader(std::string text, std::string entrypoint) : Shader(text, entrypoint, "ps_5_0") { }
	};
}

#endif