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
using Citadel::Keep::rgb;
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
/*
struct Vertex {
	Vertex(uv pos, rgb color, uv tex) : pos{ pos }, color{ color }, tex{ tex } { }
	uv pos;
	rgb color;
	uv tex;
};
*/
class HelloTriangle : public Citadel::Castle::Game {
public:
	void OnSetup() {
		SetWindowTitle(L"Hello Triangle");

		context = this->device.CreateContext2D();
		target = this->device.CreateRenderTarget();
		
		vshader = device.CreateVertexShader("struct PSInput { float4 position: SV_POSITION; float2 uv:TEXCOORD; }; PSInput VSMain(float4 position: POSITION, float4 uv: TEXCOORD) { PSInput result; result.position = position; result.uv = uv; return result; }", "VSMain", "vs_5_0");
		pshader = device.CreatePixelShader( "struct PSInput { float4 position: SV_POSITION; float2 uv:TEXCOORD; }; Texture2D gTexture: register(t0); SamplerState gSampler: register(s0); float4 PSMain(PSInput input) : SV_TARGET { return gTexture.Sample(gSampler, input.uv); }", "PSMain", "ps_5_0");
		
		//vshader = device.CreateVertexShader("struct PSInput { float4 position: SV_POSITION; float4 color: COLOR; }; PSInput VSMain(float2 position: POSITION, float3 rgb: COLOR) { PSInput result; result.position = float4(position,0,1); result.color = float4(rgb, 1); return result; }", "VSMain", "vs_5_0");
		//pshader = device.CreatePixelShader("struct PSInput { float4 position: SV_POSITION; float4 color: COLOR; }; float4 PSMain(PSInput input) : SV_TARGET { return input.color; }", "PSMain", "ps_5_0");

		//vshader = device.CreateVertexShader("AwIjBwAAAQAIAA0ANgAAAAAAAAARAAIAAQAAAAsABgABAAAAR0xTTC5zdGQuNDUwAAAAAA4AAwAAAAAAAQAAAA8ACAAAAAAABAAAAG1haW4AAAAAIgAAACYAAAAxAAAAAwADAAIAAADCAQAABAAJAEdMX0FSQl9zZXBhcmF0ZV9zaGFkZXJfb2JqZWN0cwAABAAKAEdMX0dPT0dMRV9jcHBfc3R5bGVfbGluZV9kaXJlY3RpdmUAAAQACABHTF9HT09HTEVfaW5jbHVkZV9kaXJlY3RpdmUABQAEAAQAAABtYWluAAAAAAUABQAMAAAAcG9zaXRpb25zAAAABQAEABcAAABjb2xvcnMAAAUABgAgAAAAZ2xfUGVyVmVydGV4AAAAAAYABgAgAAAAAAAAAGdsX1Bvc2l0aW9uAAYABwAgAAAAAQAAAGdsX1BvaW50U2l6ZQAAAAAGAAcAIAAAAAIAAABnbF9DbGlwRGlzdGFuY2UABgAHACAAAAADAAAAZ2xfQ3VsbERpc3RhbmNlAAUAAwAiAAAAAAAAAAUABgAmAAAAZ2xfVmVydGV4SW5kZXgAAAUABQAxAAAAZnJhZ0NvbG9yAAAASAAFACAAAAAAAAAACwAAAAAAAABIAAUAIAAAAAEAAAALAAAAAQAAAEgABQAgAAAAAgAAAAsAAAADAAAASAAFACAAAAADAAAACwAAAAQAAABHAAMAIAAAAAIAAABHAAQAJgAAAAsAAAAqAAAARwAEADEAAAAeAAAAAAAAABMAAgACAAAAIQADAAMAAAACAAAAFgADAAYAAAAgAAAAFwAEAAcAAAAGAAAAAgAAABUABAAIAAAAIAAAAAAAAAArAAQACAAAAAkAAAADAAAAHAAEAAoAAAAHAAAACQAAACAABAALAAAABgAAAAoAAAA7AAQACwAAAAwAAAAGAAAAKwAEAAYAAAANAAAAAAAAACsABAAGAAAADgAAAAAAAL8sAAUABwAAAA8AAAANAAAADgAAACsABAAGAAAAEAAAAAAAAD8sAAUABwAAABEAAAAQAAAAEAAAACwABQAHAAAAEgAAAA4AAAAQAAAALAAGAAoAAAATAAAADwAAABEAAAASAAAAFwAEABQAAAAGAAAAAwAAABwABAAVAAAAFAAAAAkAAAAgAAQAFgAAAAYAAAAVAAAAOwAEABYAAAAXAAAABgAAACsABAAGAAAAGAAAAAAAgD8sAAYAFAAAABkAAAAYAAAADQAAAA0AAAAsAAYAFAAAABoAAAANAAAAGAAAAA0AAAAsAAYAFAAAABsAAAANAAAADQAAABgAAAAsAAYAFQAAABwAAAAZAAAAGgAAABsAAAAXAAQAHQAAAAYAAAAEAAAAKwAEAAgAAAAeAAAAAQAAABwABAAfAAAABgAAAB4AAAAeAAYAIAAAAB0AAAAGAAAAHwAAAB8AAAAgAAQAIQAAAAMAAAAgAAAAOwAEACEAAAAiAAAAAwAAABUABAAjAAAAIAAAAAEAAAArAAQAIwAAACQAAAAAAAAAIAAEACUAAAABAAAAIwAAADsABAAlAAAAJgAAAAEAAAAgAAQAKAAAAAYAAAAHAAAAIAAEAC4AAAADAAAAHQAAACAABAAwAAAAAwAAABQAAAA7AAQAMAAAADEAAAADAAAAIAAEADMAAAAGAAAAFAAAADYABQACAAAABAAAAAAAAAADAAAA+AACAAUAAAA+AAMADAAAABMAAAA+AAMAFwAAABwAAAA9AAQAIwAAACcAAAAmAAAAQQAFACgAAAApAAAADAAAACcAAAA9AAQABwAAACoAAAApAAAAUQAFAAYAAAArAAAAKgAAAAAAAABRAAUABgAAACwAAAAqAAAAAQAAAFAABwAdAAAALQAAACsAAAAsAAAADQAAABgAAABBAAUALgAAAC8AAAAiAAAAJAAAAD4AAwAvAAAALQAAAD0ABAAjAAAAMgAAACYAAABBAAUAMwAAADQAAAAXAAAAMgAAAD0ABAAUAAAANQAAADQAAAA+AAMAMQAAADUAAAD9AAEAOAABAA==");
		//pshader = device.CreatePixelShader("AwIjBwAAAQAIAA0AEwAAAAAAAAARAAIAAQAAAAsABgABAAAAR0xTTC5zdGQuNDUwAAAAAA4AAwAAAAAAAQAAAA8ABwAEAAAABAAAAG1haW4AAAAACQAAAAwAAAAQAAMABAAAAAcAAAADAAMAAgAAAMIBAAAEAAkAR0xfQVJCX3NlcGFyYXRlX3NoYWRlcl9vYmplY3RzAAAEAAoAR0xfR09PR0xFX2NwcF9zdHlsZV9saW5lX2RpcmVjdGl2ZQAABAAIAEdMX0dPT0dMRV9pbmNsdWRlX2RpcmVjdGl2ZQAFAAQABAAAAG1haW4AAAAABQAFAAkAAABvdXRDb2xvcgAAAAAFAAUADAAAAGZyYWdDb2xvcgAAAEcABAAJAAAAHgAAAAAAAABHAAQADAAAAB4AAAAAAAAAEwACAAIAAAAhAAMAAwAAAAIAAAAWAAMABgAAACAAAAAXAAQABwAAAAYAAAAEAAAAIAAEAAgAAAADAAAABwAAADsABAAIAAAACQAAAAMAAAAXAAQACgAAAAYAAAADAAAAIAAEAAsAAAABAAAACgAAADsABAALAAAADAAAAAEAAAArAAQABgAAAA4AAAAAAIA/NgAFAAIAAAAEAAAAAAAAAAMAAAD4AAIABQAAAD0ABAAKAAAADQAAAAwAAABRAAUABgAAAA8AAAANAAAAAAAAAFEABQAGAAAAEAAAAA0AAAABAAAAUQAFAAYAAAARAAAADQAAAAIAAABQAAcABwAAABIAAAAPAAAAEAAAABEAAAAOAAAAPgADAAkAAAASAAAA/QABADgAAQA=");
	
		pipeline = this->device.CreatePipeline(vshader, pshader);

		float aspectRatio = 1280.0f / 720.0f;
		Vertex vertices[] = {
			{{ 0.0f, 0.25f * aspectRatio, 0.0f }, { 0.5f, 0.0f }},
			{{ 0.25f, -0.25f * aspectRatio, 0.0f }, { 1.0f, 1.0f }},
			{{ -0.25f, -0.25f * aspectRatio, 0.0f }, { 0.0f, 1.0f }}
		};
/*
		Vertex vertices[] = {
			{{0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
			{{0.0f, 0.5f}, {1.0f, 1.0f, 1.0f}, {0.5f, 0.0f}},
			
			{{-0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}
		};
*/		
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
