#ifndef __DX12__CONTEXT_H__
#define __DX12__CONTEXT_H__

#include <wrl/client.h>
using Microsoft::WRL::ComPtr;
#include <d3d12.h>

#include "../context/Context.h"
#include "../context/Context2D.h"
#include "../context/Context3D.h"


namespace Citadel::Watchtower::DX12 {
	struct ContextData {
		ComPtr<ID3D12CommandAllocator> allocators[2];
		ComPtr<ID3D12GraphicsCommandList> commandList;
	};
}

#endif
