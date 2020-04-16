#include "castle/Game.h"
#include "watchtower/Shader.h"
#include "watchtower/RenderTarget.h"
#include "keep/types.h"
#include "keep/Chrono.h"

typedef uint32_t u32;
typedef float real;

using Citadel::Keep::real3;
using Citadel::Keep::rgba;
using Citadel::Keep::real;

using Citadel::Keep::Timer;

using Citadel::Watchtower::VertexShader;
using Citadel::Watchtower::PixelShader;
using Citadel::Watchtower::Pipeline;
using Citadel::Watchtower::VertexBuffer;

#include <sstream>
#include <iomanip>

struct Vertex {
	Vertex(real3 pos, rgba color) : pos{ pos }, color{ color } { }
	real3 pos;
	rgba color;
};


class HelloTriangle : public Citadel::Castle::Game {
public:
	void OnSetup() {
		SetWindowTitle(L"Hello Triangle");

		vshader = Citadel::Watchtower::VertexShader("struct PSInput { float4 position: SV_POSITION; float4 color: COLOR; }; PSInput VSMain(float4 position: POSITION, float4 color: COLOR) { PSInput result; result.position = position; result.color = color; return result; }", "VSMain");
		pshader = Citadel::Watchtower::PixelShader( "struct PSInput { float4 position: SV_POSITION; float4 color: COLOR; }; float4 PSMain(PSInput input) : SV_TARGET { return input.color; }", "PSMain");

		pipeline = this->device.CreatePipeline(vshader, pshader);

		Vertex vertices[] = {
			{{ 0.0f, 0.25f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }},
			{{ 0.25f, -0.25f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }},
			{{ -0.25f, -0.25f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }}
		};
		
		// Using the vertex information, create a vertex buffer.
		this->vbuffer = this->device.CreateVertexBuffer(vertices, sizeof(vertices), sizeof(Vertex));

		timer.Reset();
	}

	void OnShutdown() {
	}

	void OnUpdate() {
		timer.Tick();
		CalculateStats();
	}

	void OnRender() {
		this->device.UsingPipeline(pipeline).DrawTriangleList(vbuffer);
	}
private:

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


	VertexShader vshader;
	PixelShader pshader;
	Pipeline pipeline;
	VertexBuffer vbuffer;

	u64 frameCount;
	Timer timer;
};

REGISTER_GAME_TYPE(HelloTriangle);
