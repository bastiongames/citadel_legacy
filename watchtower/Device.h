#ifndef __DEVICE_H__
#define __DEVICE_H__

#include "keep/Types.h"

#include "watchtower/Swapchain.h"
#include "watchtower/SwapchainDescriptor.h"

#include "watchtower/CommandQueue.h"
#include "watchtower/CommandQueueDescriptor.h"

#include "watchtower/CommandBuffer.h"
#include "watchtower/CommandBufferDescriptor.h"

#include "watchtower/RenderTarget.h"
#include "watchtower/RenderPass.h"

#include "watchtower/Buffers/Buffer.h"
#include "watchtower/Buffers/BufferDescriptor.h"

#include "watchtower/Shaders/Shader.h"
#include "watchtower/Shaders/ShaderDescriptor.h"
#include "watchtower/Shaders/ShaderProgram.h"
#include "watchtower/Shaders/ShaderProgramDescriptor.h"

#include "watchtower/Pipelines/PipelineLayout.h"
#include "watchtower/Pipelines/PipelineLayoutDescriptor.h"
#include "watchtower/Pipelines/GraphicsPipeline.h"
#include "watchtower/Pipelines/GraphicsPipelineDescriptor.h"

#include "watchtower/Textures/Texture.h"
#include "watchtower/Textures/TextureDescriptor.h"

#include "watchtower/ResourceHeap.h"

#include <Windows.h>

namespace Citadel::Watchtower {
	class Device {
	public:
		static Citadel::Keep::SPtr<Device> CreateDevice(HWND hwnd);

		virtual PSwapchain CreateSwapchain(const SwapchainDescriptor& descriptor) = 0;
		virtual PCommandQueue CreateCommandQueue(const CommandQueueDescriptor& descriptor) = 0;
		virtual PCommandBuffer CreateCommandBuffer(const CommandBufferDescriptor& descriptor) = 0;
		virtual PRenderPass CreateRenderPass(const PRenderTarget renderTarget) = 0;
		virtual Buffers::PBuffer CreateBuffer(const Buffers::BufferDescriptor& descriptor, void* data) = 0;
		virtual Shaders::PShader CreateShader(const Shaders::ShaderDescriptor& descriptor) = 0;
		virtual Shaders::PShaderProgram CreateShaderProgram(const Shaders::ShaderProgramDescriptor& descriptor) = 0;
		virtual Pipelines::PPipelineLayout CreatePipelineLayout(const Pipelines::PipelineLayoutDescriptor& descriptor) = 0;
		virtual Pipelines::PGraphicsPipeline CreateGraphicsPipeline(const Pipelines::GraphicsPipelineDescriptor& descriptor) = 0;
		virtual Textures::PTexture CreateTexture(const Textures::TextureDescriptor& descriptor) = 0;
		virtual PResourceHeap CreateResourceHeap(const ResourceHeapDescriptor& descriptor) = 0;
	};

	using PDevice = Citadel::Keep::SPtr<Device>;
}

#endif