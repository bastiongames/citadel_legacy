#include "HelloCube.h"
#include "watchtower/Buffers/VertexFormat.h"
#include "abacus/Matrix/Matrix4x4.h"
#include "abacus/Matrix/MatrixHelpers.h"
#include "abacus/Vector/Vector3D.h"

REGISTER_GAME_TYPE(HelloCube);

struct Vertex {
	Citadel::Keep::real3 pos;
	Citadel::Keep::rgba color;
};

void
HelloCube::OnSetup() {
	Citadel::Watchtower::SwapchainDescriptor swapchainDescriptor{ 1280, 720, 3 };
	swapchain = device->CreateSwapchain(swapchainDescriptor);

	Citadel::Watchtower::CommandQueueDescriptor commandQueueDescriptor{};
	commandQueue = device->CreateCommandQueue(commandQueueDescriptor);

	Citadel::Watchtower::CommandBufferDescriptor commandBufferDescriptor{};
	commandBuffer = device->CreateCommandBuffer(commandBufferDescriptor);

	renderPass = device->CreateRenderPass(swapchain);

	Citadel::Watchtower::Buffers::VertexFormat vertexFormat;
	vertexFormat.AppendAttribute({ "position", BufferFormat::RGB32Float });
	vertexFormat.AppendAttribute({ "color", BufferFormat::RGBA32Float });
	vertexFormat.SetStride(sizeof(Vertex));

	Vertex vertices[] = {
		{{ -1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }},
		{{ -1.0f, -1.0f,  1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }},
		{{ -1.0f,  1.0f, -1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }},
		{{ -1.0f,  1.0f,  1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f }},
		{{  1.0f, -1.0f, -1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }},
		{{  1.0f, -1.0f,  1.0f }, { 1.0f, 0.0f, 1.0f, 1.0f }},
		{{  1.0f,  1.0f, -1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f }},
		{{  1.0f,  1.0f,  1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }},
	};

	Citadel::Watchtower::Buffers::BufferDescriptor vertexBufferDescriptor{};
	vertexBufferDescriptor.size = sizeof(vertices);
	vertexBufferDescriptor.bindFlags = BindFlags::VertexBuffer;
	vertexBufferDescriptor.vertexAttributes = vertexFormat.attributes;
	vertexBuffer = device->CreateBuffer(vertexBufferDescriptor, vertices);

	Citadel::Keep::u16 indices[] = {
		// Front
		0, 2, 4,
		4, 2, 6,

		// Back
		3, 1, 7,
		1, 5, 7,
		
		// Top
		2, 3, 7,
		6, 2, 7,

		// Bottom
		1, 0, 4,
		1, 4, 5,

		// Right
		4, 6, 7,
		5, 4, 7,
		
		// Left
		0, 1, 2,
		1, 3, 2,

	};

	Citadel::Watchtower::Buffers::BufferDescriptor indexBufferDescriptor{};
	indexBufferDescriptor.size = sizeof(indices);
	indexBufferDescriptor.bindFlags = BindFlags::IndexBuffer;
	indexBuffer = device->CreateBuffer(indexBufferDescriptor, indices);

	mvp.model = Citadel::Abacus::Matrix::Matrix4x4::MakeIdentity();
	mvp.view = Citadel::Abacus::Matrix::LookAt({ 2, 2, -5 }, { 0, 0, 0 }, { 0, 1, 0 });
	mvp.proj = Citadel::Abacus::Matrix::PerspectiveFOV(0.78539816f, 1280.0f/720.0f, 0.1f, 100.0f);
	
	Citadel::Watchtower::Buffers::BufferDescriptor constantBufferDescriptor{};
	constantBufferDescriptor.size = sizeof(mvp);
	constantBufferDescriptor.bindFlags = BindFlags::ConstantBuffer;
	constantBuffer = device->CreateBuffer(constantBufferDescriptor, &mvp);
#ifdef Vulkan
	Citadel::Watchtower::Shaders::ShaderDescriptor vsDesc{ Citadel::Watchtower::Shaders::ShaderType::Vertex, "AwIjBwAAAQAIAA0ALwAAAAAAAAARAAIAAQAAAAsABgABAAAAR0xTTC5zdGQuNDUwAAAAAA4AAwAAAAAAAQAAAA8ACQAAAAAABAAAAG1haW4AAAAADQAAACEAAAArAAAALQAAAAMAAwACAAAAwgEAAAQACQBHTF9BUkJfc2VwYXJhdGVfc2hhZGVyX29iamVjdHMAAAQACgBHTF9HT09HTEVfY3BwX3N0eWxlX2xpbmVfZGlyZWN0aXZlAAAEAAgAR0xfR09PR0xFX2luY2x1ZGVfZGlyZWN0aXZlAAUABAAEAAAAbWFpbgAAAAAFAAYACwAAAGdsX1BlclZlcnRleAAAAAAGAAYACwAAAAAAAABnbF9Qb3NpdGlvbgAGAAcACwAAAAEAAABnbF9Qb2ludFNpemUAAAAABgAHAAsAAAACAAAAZ2xfQ2xpcERpc3RhbmNlAAYABwALAAAAAwAAAGdsX0N1bGxEaXN0YW5jZQAFAAMADQAAAAAAAAAFAAMAEQAAAE1WUAAGAAUAEQAAAAAAAABtb2RlbAAAAAYABQARAAAAAQAAAHZpZXcAAAAABgAFABEAAAACAAAAcHJvagAAAAAFAAMAEwAAAG12cAAFAAUAIQAAAGluUG9zaXRpb24AAAUABQArAAAAb3V0Q29sb3IAAAAABQAEAC0AAABpbkNvbG9yAEgABQALAAAAAAAAAAsAAAAAAAAASAAFAAsAAAABAAAACwAAAAEAAABIAAUACwAAAAIAAAALAAAAAwAAAEgABQALAAAAAwAAAAsAAAAEAAAARwADAAsAAAACAAAASAAEABEAAAAAAAAABQAAAEgABQARAAAAAAAAACMAAAAAAAAASAAFABEAAAAAAAAABwAAABAAAABIAAQAEQAAAAEAAAAFAAAASAAFABEAAAABAAAAIwAAAEAAAABIAAUAEQAAAAEAAAAHAAAAEAAAAEgABAARAAAAAgAAAAUAAABIAAUAEQAAAAIAAAAjAAAAgAAAAEgABQARAAAAAgAAAAcAAAAQAAAARwADABEAAAACAAAARwAEABMAAAAiAAAAAAAAAEcABAATAAAAIQAAAAAAAABHAAQAIQAAAB4AAAAAAAAARwAEACsAAAAeAAAAAAAAAEcABAAtAAAAHgAAAAEAAAATAAIAAgAAACEAAwADAAAAAgAAABYAAwAGAAAAIAAAABcABAAHAAAABgAAAAQAAAAVAAQACAAAACAAAAAAAAAAKwAEAAgAAAAJAAAAAQAAABwABAAKAAAABgAAAAkAAAAeAAYACwAAAAcAAAAGAAAACgAAAAoAAAAgAAQADAAAAAMAAAALAAAAOwAEAAwAAAANAAAAAwAAABUABAAOAAAAIAAAAAEAAAArAAQADgAAAA8AAAAAAAAAGAAEABAAAAAHAAAABAAAAB4ABQARAAAAEAAAABAAAAAQAAAAIAAEABIAAAACAAAAEQAAADsABAASAAAAEwAAAAIAAAArAAQADgAAABQAAAACAAAAIAAEABUAAAACAAAAEAAAACsABAAOAAAAGAAAAAEAAAAXAAQAHwAAAAYAAAADAAAAIAAEACAAAAABAAAAHwAAADsABAAgAAAAIQAAAAEAAAArAAQABgAAACMAAAAAAIA/IAAEACkAAAADAAAABwAAADsABAApAAAAKwAAAAMAAAAgAAQALAAAAAEAAAAHAAAAOwAEACwAAAAtAAAAAQAAADYABQACAAAABAAAAAAAAAADAAAA+AACAAUAAABBAAUAFQAAABYAAAATAAAAFAAAAD0ABAAQAAAAFwAAABYAAABBAAUAFQAAABkAAAATAAAAGAAAAD0ABAAQAAAAGgAAABkAAACSAAUAEAAAABsAAAAXAAAAGgAAAEEABQAVAAAAHAAAABMAAAAPAAAAPQAEABAAAAAdAAAAHAAAAJIABQAQAAAAHgAAABsAAAAdAAAAPQAEAB8AAAAiAAAAIQAAAFEABQAGAAAAJAAAACIAAAAAAAAAUQAFAAYAAAAlAAAAIgAAAAEAAABRAAUABgAAACYAAAAiAAAAAgAAAFAABwAHAAAAJwAAACQAAAAlAAAAJgAAACMAAACRAAUABwAAACgAAAAeAAAAJwAAAEEABQApAAAAKgAAAA0AAAAPAAAAPgADACoAAAAoAAAAPQAEAAcAAAAuAAAALQAAAD4AAwArAAAALgAAAP0AAQA4AAEA" };
	Citadel::Watchtower::Shaders::ShaderDescriptor psDesc{ Citadel::Watchtower::Shaders::ShaderType::Fragment, "AwIjBwAAAQAIAA0ADQAAAAAAAAARAAIAAQAAAAsABgABAAAAR0xTTC5zdGQuNDUwAAAAAA4AAwAAAAAAAQAAAA8ABwAEAAAABAAAAG1haW4AAAAACQAAAAsAAAAQAAMABAAAAAcAAAADAAMAAgAAAMIBAAAEAAkAR0xfQVJCX3NlcGFyYXRlX3NoYWRlcl9vYmplY3RzAAAEAAoAR0xfR09PR0xFX2NwcF9zdHlsZV9saW5lX2RpcmVjdGl2ZQAABAAIAEdMX0dPT0dMRV9pbmNsdWRlX2RpcmVjdGl2ZQAFAAQABAAAAG1haW4AAAAABQAFAAkAAABvdXRDb2xvcgAAAAAFAAQACwAAAGluQ29sb3IARwAEAAkAAAAeAAAAAAAAAEcABAALAAAAHgAAAAAAAAATAAIAAgAAACEAAwADAAAAAgAAABYAAwAGAAAAIAAAABcABAAHAAAABgAAAAQAAAAgAAQACAAAAAMAAAAHAAAAOwAEAAgAAAAJAAAAAwAAACAABAAKAAAAAQAAAAcAAAA7AAQACgAAAAsAAAABAAAANgAFAAIAAAAEAAAAAAAAAAMAAAD4AAIABQAAAD0ABAAHAAAADAAAAAsAAAA+AAMACQAAAAwAAAD9AAEAOAABAA==" };
#else
	Citadel::Watchtower::Shaders::ShaderDescriptor vsDesc{ Citadel::Watchtower::Shaders::ShaderType::Vertex, "struct PSInput { float4 position: SV_POSITION; float4 color: COLOR; }; cbuffer MVP : register(b0) { float4x4 model; float4x4 view; float4x4 proj; } PSInput VSMain(float4 position: POSITION, float4 color: COLOR) { PSInput result; result.position = mul(proj, mul(view, mul(model, position))); result.color = color; return result; }", "VSMain", "vs_5_0" };
	Citadel::Watchtower::Shaders::ShaderDescriptor psDesc{ Citadel::Watchtower::Shaders::ShaderType::Fragment, "struct PSInput { float4 position: SV_POSITION; float4 color: COLOR; }; float4 PSMain(PSInput input) : SV_TARGET { return input.color; }", "PSMain", "ps_5_0" };
#endif
	vsDesc.vertex.input = vertexFormat.attributes;

	auto vs = device->CreateShader(vsDesc);
	auto ps = device->CreateShader(psDesc);
	
	Citadel::Watchtower::Shaders::ShaderProgramDescriptor shaderProgramDescriptor{};
	shaderProgramDescriptor.vertexShader = vs;
	shaderProgramDescriptor.pixelShader = ps;
	auto shaderProgram = device->CreateShaderProgram(shaderProgramDescriptor);

	Citadel::Watchtower::Pipelines::PPipelineLayout pipelineLayout;
	{
		Citadel::Watchtower::Pipelines::PipelineLayoutDescriptor pipelineLayoutDescriptor;
		{
			pipelineLayoutDescriptor.bindings = {
				{Citadel::Watchtower::Flags::ResourceType::Buffer, Citadel::Watchtower::Flags::BindFlags::ConstantBuffer, Citadel::Watchtower::Flags::StageFlags::VertexStage, 0}
			};
		}
		pipelineLayout = device->CreatePipelineLayout(pipelineLayoutDescriptor);

		Citadel::Watchtower::Pipelines::GraphicsPipelineDescriptor pipelineDescriptor{};
		pipelineDescriptor.shaderProgram = shaderProgram;
		pipelineDescriptor.renderPass = renderPass;
		pipelineDescriptor.pipelineLayout = pipelineLayout;
		graphicsPipeline = device->CreateGraphicsPipeline(pipelineDescriptor);
	}

	Citadel::Watchtower::ResourceHeapDescriptor resourceHeapDescriptor{};
	resourceHeapDescriptor.pipelineLayout = pipelineLayout;
	resourceHeapDescriptor.resourceCount = 1;
	resourceHeap = device->CreateResourceHeap(resourceHeapDescriptor);
	resourceHeap->BindBuffer(constantBuffer, 0);
}  

void
HelloCube::OnShutdown() {

}

void
HelloCube::OnUpdate() {
	yTheta += 0.0001f;
	xTheta += 0.0001f;
	zTheta += 0.0001f;

	mvp.model = Citadel::Abacus::Matrix::Rotation(xTheta, yTheta, zTheta);

	constantBuffer->Update(&mvp, sizeof(mvp));
}

void
HelloCube::OnRender() {
	commandBuffer->Begin();
	commandBuffer->SetViewport({ 0,0,1280,720,0,1 }, { 0,0,1280,720 });
	commandBuffer->SetPipeline(graphicsPipeline);
	commandBuffer->SetVertexBuffer(vertexBuffer);
	commandBuffer->BeginRenderPass(renderPass);
	commandBuffer->Clear({ 0.5f, 0.5f, 0.5f, 1.0f });
	commandBuffer->SetResourceHeap(resourceHeap);

	commandBuffer->SetIndexBuffer(indexBuffer);// , Citadel::Watchtower::Formats::BufferFormat::R16UInt, 0);
	commandBuffer->DrawIndexed(36, 0);

	commandBuffer->EndRenderPass();
	commandBuffer->End();

	commandQueue->Submit(commandBuffer);

	swapchain->Swap();
}
