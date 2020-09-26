#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "keep/Types.h"

#include "watchtower/Common/Buffers/Buffer.h"
#include "watchtower/Common/Shaders/Shader.h"
#include "watchtower/Common/Shaders/ShaderProgram.h"
#include "watchtower/Common/Pipeline/PipelineState.h"

#include "watchtower/Common/Commands/CommandQueue.h"
#include "watchtower/Common/Commands/CommandBuffer.h"

#include "watchtower/Common/context/RenderContext.h"
#include "watchtower/Common/context/RenderContextDescriptor.h"
#include "watchtower/Common/Buffers/BufferDescriptor.h"
#include "watchtower/Common/Shaders/ShaderDescriptor.h"
#include "watchtower/Common/Shaders/ShaderProgramDescriptor.h"
#include "watchtower/Common/Pipeline/GraphicsPipelineDescriptor.h"

using Citadel::Keep::SPtr;
using Citadel::Keep::handle;
using Citadel::Keep::u32;

using Citadel::Watchtower::Context::RenderContextDescriptor;
using Citadel::Watchtower::Buffers::BufferDescriptor;
using Citadel::Watchtower::Shaders::ShaderDescriptor;
using Citadel::Watchtower::Shaders::ShaderProgramDescriptor;
using Citadel::Watchtower::Pipeline::GraphicsPipelineDescriptor;

using Citadel::Watchtower::Context::RenderContext;
using Citadel::Watchtower::Buffers::Buffer;
using Citadel::Watchtower::Shaders::Shader;
using Citadel::Watchtower::Shaders::ShaderProgram;
using Citadel::Watchtower::Pipeline::PipelineState;
using Citadel::Watchtower::Commands::CommandBuffer;
using Citadel::Watchtower::Commands::CommandQueue;

namespace Citadel::Watchtower {
	class Renderer {
	public:
		Renderer(handle platformData);
		void Resize(u32 width, u32 height);

		SPtr<Buffer> CreateBuffer(void* data, BufferDescriptor desciptor);
		SPtr<Shader> CreateShader(ShaderDescriptor descriptor);
		SPtr<ShaderProgram> CreateShaderProgram(ShaderProgramDescriptor descriptor);
		SPtr<PipelineState> CreatePipelineState(GraphicsPipelineDescriptor descriptor);

		SPtr<RenderContext> CreateRenderContext(RenderContextDescriptor descriptor);

		SPtr<CommandQueue> GetCommandQueue();
		SPtr<CommandBuffer> CreateCommandBuffer();

	private:
		void AcquireDevice(handle platformData);
		void ReleaseDevice();

		handle impl;
	};
}

#endif