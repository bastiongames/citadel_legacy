#include "castle/game/Game.h"

#include "watchtower/Buffers/VertexFormat.h"
#include "watchtower/Buffers/BufferDescriptor.h"

#include "watchtower/Shaders/Shader.h"
#include "watchtower/Shaders/ShaderDescriptor.h"
#include "watchtower/Shaders/ShaderProgramDescriptor.h"

#include "watchtower/Shaders/ShaderType.h"

#include "watchtower/Pipelines/GraphicsPipelineDescriptor.h"
#include "watchtower/Pipelines/PipelineLayoutDescriptor.h"

#include "watchtower/Textures/Texture.h"
#include "watchtower/Textures/TextureDescriptor.h"

#include "watchtower/ResourceHeap.h"

#include "abacus/Matrix/Matrix4x4.h"

struct Vertex {
	Citadel::Keep::real2 pos;
	Citadel::Keep::uv tex;
	Citadel::Keep::rgba color;
};

struct MVP {
	Citadel::Abacus::Matrix::Matrix4x4 model;
	Citadel::Abacus::Matrix::Matrix4x4 view;
	Citadel::Abacus::Matrix::Matrix4x4 proj;
};

class HelloTriangle : public Citadel::Castle::Game {
public:
	void OnSetup() {
		SetWindowTitle(L"Hello Triangle");
		
		Citadel::Watchtower::SwapchainDescriptor descriptor{ 1280, 720, 3 };
		swapchain = device->CreateSwapchain(descriptor);

		Citadel::Watchtower::CommandQueueDescriptor commandQueueDesc{};
		commandQueue = device->CreateCommandQueue(commandQueueDesc);

		Citadel::Watchtower::CommandBufferDescriptor commandBufferDesc{};
		commandBuffer = device->CreateCommandBuffer(commandBufferDesc);
		
		renderPass = device->CreateRenderPass(swapchain);

		const Citadel::Keep::real s = 0.5f;

		Vertex vertices[] = {
			{ { 0.0f, s }, { 0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
			{ { s, -s }, { 1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
			{ { -s, -s }, { 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }
		};

		Citadel::Watchtower::Buffers::VertexFormat vertexFormat;
		vertexFormat.AppendAttribute({ "position", BufferFormat::RG32Float });
		vertexFormat.AppendAttribute({ "texcoord", BufferFormat::RG32Float });
		vertexFormat.AppendAttribute({ "color", BufferFormat::RGBA32Float });
		vertexFormat.SetStride(sizeof(Vertex));

		Citadel::Watchtower::Buffers::BufferDescriptor vertexBufferDescriptor;
		vertexBufferDescriptor.size = sizeof(vertices);
		vertexBufferDescriptor.bindFlags = BindFlags::VertexBuffer;
		vertexBufferDescriptor.vertexAttributes = vertexFormat.attributes;  

		vertexBuffer = device->CreateBuffer(vertexBufferDescriptor, vertices);

		mvp.model = Citadel::Abacus::Matrix::Matrix4x4::MakeIdentity();
		mvp.view = Citadel::Abacus::Matrix::Matrix4x4::MakeIdentity();
		mvp.proj = Citadel::Abacus::Matrix::Matrix4x4::MakeIdentity();
		
		Citadel::Watchtower::Buffers::BufferDescriptor constantBufferDescriptor;
		constantBufferDescriptor.size = sizeof(mvp);
		constantBufferDescriptor.bindFlags = BindFlags::ConstantBuffer;
		constantBuffer = device->CreateBuffer(constantBufferDescriptor, &mvp);

#ifndef Vulkan
		Citadel::Watchtower::Shaders::ShaderDescriptor vsDesc{ Citadel::Watchtower::Shaders::ShaderType::Vertex, "struct PSInput {float4 position: SV_POSITION;float2 uv: TEXCOORD;float4 color: COLOR;}; cbuffer MVP : register(b0) { float4x4 model; float4x4 view; float4x4 proj; }  PSInput VSMain(float4 position: POSITION, float2 uv: TEXCOORD, float4 color: COLOR) {PSInput result;result.position = position;result.uv = uv;result.color = color;return result;}", "VSMain", "vs_5_0" };
		Citadel::Watchtower::Shaders::ShaderDescriptor psDesc{ Citadel::Watchtower::Shaders::ShaderType::Pixel, "struct PSInput {float4 position: SV_POSITION;float2 uv: TEXCOORD;float4 color: COLOR;};Texture2D gTexture: register(t0);SamplerState gSampler: register(s0);float4 PSMain(PSInput input) : SV_TARGET {return gTexture.Sample(gSampler, input.uv) * input.color;}", "PSMain", "ps_5_0" };
#else

		Citadel::Watchtower::Shaders::ShaderDescriptor vsDesc{ Citadel::Watchtower::Shaders::ShaderType::Vertex, "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" };
		Citadel::Watchtower::Shaders::ShaderDescriptor psDesc{ Citadel::Watchtower::Shaders::ShaderType::Fragment, "AwIjBwAAAQAIAA0AHQAAAAAAAAARAAIAAQAAAAsABgABAAAAR0xTTC5zdGQuNDUwAAAAAA4AAwAAAAAAAQAAAA8ACAAEAAAABAAAAG1haW4AAAAACQAAABYAAAAaAAAAEAADAAQAAAAHAAAAAwADAAIAAADCAQAABAAJAEdMX0FSQl9zZXBhcmF0ZV9zaGFkZXJfb2JqZWN0cwAABAAKAEdMX0dPT0dMRV9jcHBfc3R5bGVfbGluZV9kaXJlY3RpdmUAAAQACABHTF9HT09HTEVfaW5jbHVkZV9kaXJlY3RpdmUABQAEAAQAAABtYWluAAAAAAUABQAJAAAAb3V0Q29sb3IAAAAABQAFAAwAAABjb2xvck1hcAAAAAAFAAYAEAAAAGNvbG9yTWFwU2FtcGxlcgAFAAQAFgAAAGluVGV4AAAABQAEABoAAABpbkNvbG9yAEcABAAJAAAAHgAAAAAAAABHAAQADAAAACIAAAAAAAAARwAEAAwAAAAhAAAAAgAAAEcABAAQAAAAIgAAAAAAAABHAAQAEAAAACEAAAABAAAARwAEABYAAAAeAAAAAAAAAEcABAAaAAAAHgAAAAEAAAATAAIAAgAAACEAAwADAAAAAgAAABYAAwAGAAAAIAAAABcABAAHAAAABgAAAAQAAAAgAAQACAAAAAMAAAAHAAAAOwAEAAgAAAAJAAAAAwAAABkACQAKAAAABgAAAAEAAAAAAAAAAAAAAAAAAAABAAAAAAAAACAABAALAAAAAAAAAAoAAAA7AAQACwAAAAwAAAAAAAAAGgACAA4AAAAgAAQADwAAAAAAAAAOAAAAOwAEAA8AAAAQAAAAAAAAABsAAwASAAAACgAAABcABAAUAAAABgAAAAIAAAAgAAQAFQAAAAEAAAAUAAAAOwAEABUAAAAWAAAAAQAAACAABAAZAAAAAQAAAAcAAAA7AAQAGQAAABoAAAABAAAANgAFAAIAAAAEAAAAAAAAAAMAAAD4AAIABQAAAD0ABAAKAAAADQAAAAwAAAA9AAQADgAAABEAAAAQAAAAVgAFABIAAAATAAAADQAAABEAAAA9AAQAFAAAABcAAAAWAAAAVwAFAAcAAAAYAAAAEwAAABcAAAA9AAQABwAAABsAAAAaAAAAhQAFAAcAAAAcAAAAGAAAABsAAAA+AAMACQAAABwAAAD9AAEAOAABAA==" };
#endif
		vsDesc.vertex.input = vertexFormat.attributes;

		auto vertexShader = device->CreateShader(vsDesc);
		auto pixelShader = device->CreateShader(psDesc);

		Citadel::Watchtower::Shaders::ShaderProgramDescriptor shaderProgramDescriptor;
		shaderProgramDescriptor.vertexShader = vertexShader;
		shaderProgramDescriptor.pixelShader = pixelShader;
		auto shaderProgram = device->CreateShaderProgram(shaderProgramDescriptor);

		// Create Pipeline
		Citadel::Watchtower::Pipelines::PPipelineLayout pipelineLayout;
		{
			Citadel::Watchtower::Pipelines::PipelineLayoutDescriptor pipelineLayoutDescriptor;
			{
				pipelineLayoutDescriptor.bindings = {
					{Citadel::Watchtower::Flags::ResourceType::Buffer, Citadel::Watchtower::Flags::BindFlags::ConstantBuffer, Citadel::Watchtower::Flags::StageFlags::VertexStage, 0},
					{Citadel::Watchtower::Flags::ResourceType::Texture, Citadel::Watchtower::Flags::BindFlags::Sampled, Citadel::Watchtower::Flags::StageFlags::PixelStage, 2},
					{Citadel::Watchtower::Flags::ResourceType::Sampler, Citadel::Watchtower::Flags::BindFlags::None, Citadel::Watchtower::Flags::StageFlags::PixelStage, 1},
				};
			}

			pipelineLayout = device->CreatePipelineLayout(pipelineLayoutDescriptor);

			Citadel::Watchtower::Pipelines::GraphicsPipelineDescriptor pipelineDescriptor;
			pipelineDescriptor.shaderProgram = shaderProgram;
			pipelineDescriptor.renderPass = renderPass;
			pipelineDescriptor.pipelineLayout = pipelineLayout;
			graphicsPipeline = device->CreateGraphicsPipeline(pipelineDescriptor);
		}

		Citadel::Watchtower::Textures::TextureDescriptor textureDescriptor;
		textureDescriptor.width = 256;
		textureDescriptor.height = 256;
		textureDescriptor.data = GenerateTextureData(256, 256, 4);
		texture = device->CreateTexture(textureDescriptor);

		Citadel::Watchtower::ResourceHeapDescriptor resourceHeapDescriptor;
		resourceHeapDescriptor.pipelineLayout = pipelineLayout;
		resourceHeapDescriptor.resourceCount = 2;
		resourceHeapDescriptor.samplerCount = 1;
		resourceHeap = device->CreateResourceHeap(resourceHeapDescriptor);
		resourceHeap->BindBuffer(constantBuffer, 0);
		resourceHeap->BindTexture(texture, 1);
		
	}

	void OnShutdown() {
	}

	void OnUpdate() {
	}

	void OnRender() {
		commandBuffer->Begin();
		commandBuffer->SetViewport({ 0,0,1280,720, 0,1 }, { 0,0,1280,720 });
		commandBuffer->SetPipeline(graphicsPipeline);
		commandBuffer->SetVertexBuffer(vertexBuffer);
		commandBuffer->BeginRenderPass(renderPass);
		commandBuffer->Clear({ 0.5f,0.5f,0.5f,1.0f });
		commandBuffer->SetResourceHeap(resourceHeap);
		commandBuffer->Draw(3, 0);
		commandBuffer->EndRenderPass();
		commandBuffer->End();

		commandQueue->Submit(commandBuffer);

		swapchain->Swap();
	}

private:

	Citadel::Watchtower::PSwapchain swapchain;
	Citadel::Watchtower::PCommandQueue commandQueue;
	Citadel::Watchtower::PCommandBuffer commandBuffer;
	Citadel::Watchtower::PRenderPass renderPass;
	Citadel::Watchtower::Buffers::PBuffer vertexBuffer;
	Citadel::Watchtower::Pipelines::PGraphicsPipeline graphicsPipeline;
	Citadel::Watchtower::Textures::PTexture texture;
	Citadel::Watchtower::PResourceHeap resourceHeap;

	Citadel::Watchtower::Buffers::PBuffer constantBuffer;

	MVP mvp;
	
	handle GenerateTextureData(u32 width, u32 height, u32 pixelSize) {
		const u32 rowPitch = width * pixelSize;
		const u32 cellPitch = rowPitch >> 3;
		const u32 cellHeight = width >> 3;
		const u32 textureSize = rowPitch * height;

		auto buf = std::shared_ptr<unsigned char>(new unsigned char[textureSize], std::default_delete<unsigned char[]>());
		Citadel::Keep::u8* pData = buf.get();

		for (u32 n = 0; n < textureSize; n += pixelSize) {
			u32 x = n % rowPitch;
			u32 y = n / rowPitch;
			u32 i = x / cellPitch;
			u32 j = y / cellHeight;

			if (i % 2 == j % 2) {
				pData[n] = pData[n + 1] = pData[n + 2] = 0x00;
				pData[n + 3] = 0xFF;
			}
			else {
				pData[n] = pData[n + 1] = pData[n + 2] = 0xFF;
				pData[n + 3] = 0xFF;
			}
		}

		return Citadel::Keep::HandleFromSPtr<Citadel::Keep::u8>(buf);
	}
	
};

REGISTER_GAME_TYPE(HelloTriangle);
