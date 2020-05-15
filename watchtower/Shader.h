#ifndef __SHADER_H__
#define __SHADER_H__

#include <string>

#include "keep/Types.h"
using Citadel::Keep::handle;

namespace Citadel::Watchtower {
	class Shader {
	public:
		Shader(handle data = nullptr);
		//Shader(std::string text, std::string entrypoint, std::string target);
		Shader(const Shader& other);
		Shader(Shader&& other) noexcept;
		~Shader() noexcept;

		Shader& operator=(const Shader& other);
		Shader& operator=(Shader&& other) noexcept;

		handle Get();

	private:
		//handle Compile(std::string text, std::string entrypoint, std::string target);

		handle impl;
	};

	class VertexShader : public Shader {
	public:
		VertexShader(handle data = nullptr) : Shader(data) { }

		VertexShader(const VertexShader& other) = default;
		VertexShader(VertexShader&& other) noexcept = default;
		~VertexShader() noexcept = default;

		VertexShader& operator=(const VertexShader& other) = default;
		VertexShader& operator=(VertexShader&& other) noexcept = default;
	};

	class PixelShader : public Shader {
	public:
		PixelShader(handle data = nullptr) : Shader(data) { }

		PixelShader(const PixelShader& other) = default;
		PixelShader(PixelShader && other) noexcept = default;
		~PixelShader() noexcept = default;

		PixelShader& operator=(const PixelShader& other) = default;
		PixelShader& operator=(PixelShader && other) noexcept = default;
	};
}

#endif