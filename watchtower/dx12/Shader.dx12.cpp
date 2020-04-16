#include "Shader.h"

#include <d3d12.h>
#include <d3dcompiler.h>
#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

using Citadel::Keep::SPtr;
using Citadel::Keep::MakeSPtr;
using Citadel::Keep::HandleFromSPtr;

using Citadel::Watchtower::DX12::ShaderData;

handle
Citadel::Watchtower::Shader::Compile(std::string text, std::string entrypoint, std::string target) {
	auto data = MakeSPtr<ShaderData>();
	ComPtr<ID3DBlob> errors;
	HRESULT hr = D3DCompile(text.c_str(), text.size(), nullptr, nullptr, nullptr, entrypoint.c_str(), target.c_str(), 0, 0, &data->bytecode, &errors);
	return HandleFromSPtr<ShaderData>(data);
}