#ifndef __DX12__SHADER_H__
#define __DX12__SHADER_H__

#include "../Shader.h"

#include <d3d12.h>
#include <d3dcompiler.h>
#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

namespace Citadel::Watchtower::DX12 {
	struct ShaderData {
		ComPtr<ID3DBlob> bytecode;
	};
}

#endif
