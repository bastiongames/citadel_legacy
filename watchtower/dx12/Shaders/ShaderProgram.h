#ifndef __DX12__SHADERPROGRAM_H__
#define __DX12__SHADERPROGRAM_H__

#include "watchtower/Shaders/ShaderProgram.h"
#include "watchtower/Shaders/ShaderProgramDescriptor.h"

#include <d3d12.h>
#include <wrl/client.h>
using Microsoft::WRL::ComPtr;

namespace Citadel::Watchtower::Shaders::DX12 {
	class ShaderProgram : public Citadel::Watchtower::Shaders::ShaderProgram{
	public:
		ShaderProgram(const ComPtr<ID3D12Device> device, const ShaderProgramDescriptor& descriptor);

		virtual void* GetBytecode(Shaders::ShaderType type);

	public:
		D3D12_INPUT_LAYOUT_DESC GetLayoutDescriptor();

	private:
		PShader vs;
		PShader ps;
	};
}

#endif