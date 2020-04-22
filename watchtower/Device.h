#ifndef __DEVICE_H__
#define __DEVICE_H__

#include "keep/Types.h"

#include "watchtower/Shader.h"
#include "watchtower/Pipeline.h"
#include "watchtower/VertexBuffer.h"
#include "watchtower/Texture.h"
#include "watchtower/RenderTarget.h"

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

		Device& BeginFrame();

		Pipeline CreatePipeline(VertexShader& vshader, PixelShader& pshader);
		Device& UsingPipeline(Pipeline& pipeline);
		
		VertexBuffer CreateVertexBuffer(void* data, u32 size, u32 stride);
		Device& DrawTriangleList(VertexBuffer& buffer);
		
		Texture CreateTexture(handle data, u32 width, u32 height, u32 pixelSize);
		Device& MapTexture(Texture& texture);

		RenderTarget CreateRenderTarget();
		Device& RenderTo(RenderTarget& target);

		void Present(RenderTarget& target);
		void FinishFrame();
		void Wait();

	private:
		handle AcquireDevice(handle platformData);
		void ReleaseDevice();

		handle impl;
	};
}

#endif