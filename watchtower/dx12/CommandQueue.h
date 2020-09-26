#ifndef __DX12__COMMANDQUEUE_H__
#define __DX12__COMMANDQUEUE_H__

#include "watchtower/CommandQueue.h"

#include <d3d12.h>
#include <wrl/client.h>
using Microsoft::WRL::ComPtr;


namespace Citadel::Watchtower::DX12 {
	class CommandQueue : public Watchtower::CommandQueue {
	public:
		CommandQueue(ComPtr<ID3D12Device> device, ComPtr<ID3D12CommandQueue> queue = nullptr);
		virtual void Submit(PCommandBuffer commandBuffer);

	private:
		ComPtr<ID3D12CommandQueue> commandQueue;
	};
}

#endif