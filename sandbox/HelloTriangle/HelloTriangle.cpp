#include "castle/game/Game.h"
#include "watchtower/Shader.h"
#include "watchtower/Texture.h"
#include "watchtower/RenderTarget.h"
#include "watchtower/context/Context2D.h"
#include "keep/types.h"
#include "keep/Chrono.h"

#include "abacus/Vector/Vector2D.h"
using Citadel::Abacus::Vector::Vector2D;

using Citadel::Keep::u8;
using Citadel::Keep::u32;
using Citadel::Keep::real3;
using Citadel::Keep::rgba;
using Citadel::Keep::uv;
using Citadel::Keep::real;
using Citadel::Keep::HandleFromSPtr;
using Citadel::Keep::MakeSPtr;

using Citadel::Keep::Timer;

using Citadel::Watchtower::Context2D;
using Citadel::Watchtower::VertexShader;
using Citadel::Watchtower::PixelShader;
using Citadel::Watchtower::Pipeline;
using Citadel::Watchtower::VertexBuffer;
using Citadel::Watchtower::Texture;
using Citadel::Watchtower::RenderTarget;

#include <sstream>
#include <iomanip>
#include <vector>

struct Vertex {
	Vertex(real3 pos, uv tex) : pos{ pos }, tex{ tex } { }
	real3 pos;
	uv tex;
};

class HelloTriangle : public Citadel::Castle::Game {
public:
	void OnSetup() {
		SetWindowTitle(L"Hello Triangle");

		context = this->device.CreateContext2D();
		target = this->device.CreateRenderTarget();

		vshader = Citadel::Watchtower::VertexShader("struct PSInput { float4 position: SV_POSITION; float2 uv:TEXCOORD; }; PSInput VSMain(float4 position: POSITION, float4 uv: TEXCOORD) { PSInput result; result.position = position; result.uv = uv; return result; }", "VSMain");
		pshader = Citadel::Watchtower::PixelShader( "struct PSInput { float4 position: SV_POSITION; float2 uv:TEXCOORD; }; Texture2D gTexture: register(t0); SamplerState gSampler: register(s0); float4 PSMain(PSInput input) : SV_TARGET { return gTexture.Sample(gSampler, input.uv); }", "PSMain");
		pipeline = this->device.CreatePipeline(vshader, pshader);

		float aspectRatio = 1024.0f / 768.0f;
		Vertex vertices[] = {
			{{ 0.0f, 0.25f * aspectRatio, 0.0f }, { 0.5f, 0.0f }},
			{{ 0.25f, -0.25f * aspectRatio, 0.0f }, { 1.0f, 1.0f }},
			{{ -0.25f, -0.25f * aspectRatio, 0.0f }, { 0.0f, 1.0f }}
		};
		
		// Using the vertex information, create a vertex buffer.
		this->vbuffer = this->device.CreateVertexBuffer(vertices, sizeof(vertices), sizeof(Vertex));
	
		auto textureData = GenerateTextureData(256, 256, 4);
		texture = this->device.CreateTexture(textureData, 256, 256, 4);

		timer.Reset();
	}

	void OnShutdown() {
	}

	void OnUpdate() {
		timer.Tick();
		CalculateStats();
	}

	void OnRender() {
		context
			.Begin(target, frameIndex)
			.UsingPipeline(pipeline)
			.PaintWith(texture)
			.DrawTriangleList(vbuffer)
			.Finish(target, frameIndex);
		device.Present(context);
		frameIndex = device.FinishFrame();
	}
private:

	handle GenerateTextureData(u32 width, u32 height, u32 pixelSize) {
		const u32 rowPitch = width * pixelSize;
		const u32 cellPitch = rowPitch >> 3;
		const u32 cellHeight = width >> 3;
		const u32 textureSize = rowPitch * height;

		auto buf = std::shared_ptr<unsigned char>(new unsigned char[textureSize], std::default_delete<unsigned char[]>());
		u8* pData = buf.get();

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

		return HandleFromSPtr<u8>(buf);
	}

	void CalculateStats() {
		static int frameCnt = 0;
		static double elapsedTime = 0.0;
		double totalTime = timer.GetTotalSeconds();
		frameCnt++;
		if ((totalTime - elapsedTime) >= 1.0f) {
			float diff = static_cast<float>(totalTime - elapsedTime);
			float fps = static_cast<float>(frameCnt) / diff;

			frameCnt = 0;
			elapsedTime = totalTime;

			std::wstringstream text;
			text << "Citadel - Hello Triangle "
				<< std::setprecision(2) << std::fixed
				<< L"    fps: " << fps;
			SetWindowTitle(text.str().c_str());
		}
	}

	u32 frameIndex = 0;
	Context2D context;
	RenderTarget target;

	VertexShader vshader;
	PixelShader pshader;
	Pipeline pipeline;
	VertexBuffer vbuffer;
	Texture texture;

	u64 frameCount;
	Timer timer;
};

REGISTER_GAME_TYPE(HelloTriangle);
