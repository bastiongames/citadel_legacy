#ifndef __DEVICE_H__
#define __DEVICE_H__

#include "keep/Types.h"

#include "watchtower/context/Context2D.h"
#include "watchtower/context/Context3D.h"

#include "watchtower/Shader.h"
#include "watchtower/Pipeline.h"
#include "watchtower/VertexBuffer.h"
#include "watchtower/Texture.h"
#include "watchtower/RenderTarget.h"
#include "watchtower/Camera.h"

using Citadel::Keep::u64;
using Citadel::Keep::u32;
using Citadel::Keep::handle;

namespace Citadel::Watchtower {

	class Device
	{
	public:
		Device();
		Device(handle data);
		Device(const Device& other);
		Device(Device&& other) noexcept;
		~Device();

		Device& operator=(const Device& other);
		Device& operator=(Device&& other) noexcept;

		Context2D CreateContext2D();
		Context3D CreateContext3D();
		
		VertexShader CreateVertexShader(std::string src, std::string entrypoint = "", std::string model = "");
		PixelShader CreatePixelShader(std::string src, std::string entrypoint = "", std::string model = "");

		Device& BeginFrame();

		Pipeline CreatePipeline(VertexShader& vshader, PixelShader& pshader);
		VertexBuffer CreateVertexBuffer(void* data, u32 size, u32 stride);
		Texture CreateTexture(handle data, u32 width, u32 height, u32 pixelSize);
		RenderTarget CreateRenderTarget();

		void Present(RenderTarget& target);
		void Present(Context& context);
		u32 FinishFrame();
		void Wait();

	private:
		handle AcquireDevice(handle platformData);
		void ReleaseDevice();

		handle impl;
	};
}

#endif