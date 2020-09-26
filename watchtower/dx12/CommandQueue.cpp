#include "CommandQueue.h"
#include "CommandBuffer.h"

namespace Citadel::Watchtower::DX12 {
	CommandQueue::CommandQueue(ComPtr<ID3D12Device> device, ComPtr<ID3D12CommandQueue> queue) {
		D3D12_COMMAND_QUEUE_DESC queueDesc{};
		queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		if (queue == nullptr)
			device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&commandQueue));
		else
			commandQueue = queue;
	}

	void
	CommandQueue::Submit(PCommandBuffer commandBuffer) {
		auto commandbuffer = Keep::CastSPtr<Watchtower::CommandBuffer, DX12::CommandBuffer>(commandBuffer);
		commandbuffer->Record(commandQueue);
	}
}