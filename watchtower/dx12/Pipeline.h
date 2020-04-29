#ifndef __DX12_PIPELINE_H__
#define __DX12_PIPELINE_H__

#include <d3d12.h>
#include "d3dx12.h"

#include <wrl/client.h>
using Microsoft::WRL::ComPtr;

#include "../Pipeline.h"


namespace Citadel::Watchtower::DX12 {
	struct PipelineData {
		ComPtr<ID3D12PipelineState> pipelineState;
		CD3DX12_VIEWPORT viewport;
		CD3DX12_RECT scissorRect;

		ComPtr<ID3D12RootSignature> signature;
	};
}

#endif