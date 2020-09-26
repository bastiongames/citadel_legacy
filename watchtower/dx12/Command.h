#ifndef __DX12__COMMAND_H__
#define __DX12__COMMAND_H__

#include <d3d12.h>
#include <wrl/client.h>
using Microsoft::WRL::ComPtr;

namespace Citadel::Watchtower {
	class Command {
	public:
		virtual void Record(const ComPtr<ID3D12GraphicsCommandList> commandList) = 0;
	};
}

#endif