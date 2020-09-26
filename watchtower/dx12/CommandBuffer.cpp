#include "CommandBuffer.h"

#include "Command.h"

// Keeping this dependency since we have to track the RenderPass for multiple calls
// Maybe do a BeginEndCommand?
#include "RenderPass.h"

#include "d3dx12.h"

namespace Citadel::Watchtower::DX12 {
	CommandBuffer::CommandBuffer(ComPtr<ID3D12Device> device) {
		HRESULT hr = S_OK;
		Keep::u32 numLists = 3;

		allocators.resize(numLists);
		commandLists.resize(numLists);
		fenceValues.resize(numLists);

		for (Keep::u32 i = 0; i < numLists; ++i) {
			hr = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&allocators[i]));
			hr = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, allocators[i].Get(), nullptr, IID_PPV_ARGS(&commandLists[i]));

			commandLists[i]->Close();
			fenceValues[i] = i;
		}

		device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
		fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		
		listIndex = -1;
		NextList();
	}

	void
	CommandBuffer::Begin() {
		allocator->Reset();
		commandList->Reset(allocator.Get(), nullptr);
	}

	void
	CommandBuffer::SetViewport(Keep::Frustum viewport, Keep::Rect scissor) {
		D3D12_VIEWPORT viewports[] = {
			{viewport.x, viewport.y, viewport.w, viewport.h, viewport.n, viewport.f}
		};
		commandList->RSSetViewports(1, viewports);

		D3D12_RECT scissors[] = {
			{(LONG)scissor.x, (LONG)scissor.y, (LONG)scissor.w, (LONG)scissor.h}
		};
		commandList->RSSetScissorRects(1, scissors);
	}

	void
	CommandBuffer::SetPipeline(const Pipelines::PGraphicsPipeline pipeline) {
		auto command = Keep::CastSPtr<Pipelines::GraphicsPipeline, Command>(pipeline);
		command->Record(this->commandList);
	}

	void
	CommandBuffer::SetVertexBuffer(const Buffers::PBuffer vertexBuffer) {
		auto command = Keep::CastSPtr<Buffers::Buffer, Command>(vertexBuffer);
		command->Record(commandList);
	}

	void
		CommandBuffer::SetIndexBuffer(const Buffers::PBuffer indexBuffer) {
		auto command = Keep::CastSPtr<Buffers::Buffer, Command>(indexBuffer);
		command->Record(commandList);
	}

	void
	CommandBuffer::BeginRenderPass(const PRenderPass renderPass) {
		auto renderpass = Keep::CastSPtr<Watchtower::RenderPass, DX12::RenderPass>(renderPass);
		renderTarget = Keep::CastSPtr<Watchtower::RenderTarget, DX12::RenderTarget>(renderpass->GetTarget());
		
		auto handle = renderTarget->GetRenderTargetHandle();
		auto frame = renderTarget->GetBackBuffer();
		
		auto transition = CD3DX12_RESOURCE_BARRIER::Transition(
			frame.Get(),
			D3D12_RESOURCE_STATE_PRESENT,
			D3D12_RESOURCE_STATE_RENDER_TARGET
		);
		
		commandList->ResourceBarrier(1, &transition);
		commandList->OMSetRenderTargets(1, &handle, false, nullptr);
		
	}

	void
	CommandBuffer::Clear(Keep::rgba clearColor) {
		float color[] = { clearColor.r, clearColor.g, clearColor.b, clearColor.a };

		auto handle = renderTarget->GetRenderTargetHandle();
		commandList->ClearRenderTargetView(handle, color, 0, nullptr);
	}

	void
	CommandBuffer::SetResourceHeap(const PResourceHeap resourceHeap) {
		auto command = Keep::CastSPtr<ResourceHeap, Command>(resourceHeap);
		command->Record(this->commandList);
	}

	void
	CommandBuffer::Draw(Keep::u32 count, Keep::u32 offset) {
		commandList->DrawInstanced(count, 1, offset, 0);
	}
	
	void
	CommandBuffer::DrawIndexed(Keep::u32 count, Keep::u32 offset) {
		commandList->DrawIndexedInstanced(count, 1, offset, 0, 0);
	}

	void
	CommandBuffer::EndRenderPass() { }

	void
	CommandBuffer::End() {
		auto frame = renderTarget->GetBackBuffer();

		auto transition = CD3DX12_RESOURCE_BARRIER::Transition(
			frame.Get(),
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			D3D12_RESOURCE_STATE_PRESENT
		);
		commandList->ResourceBarrier(1, &transition);

		commandList->Close();
	}

	void
	CommandBuffer::Record(ComPtr<ID3D12CommandQueue> queue) {
		ID3D12CommandList* commandLists[] = { commandList.Get() };
		queue->ExecuteCommandLists(1, commandLists);

		NextList();
	}

	void
	CommandBuffer::NextList() {
		listIndex = ((Keep::u64)listIndex + 1) % commandLists.size();
		allocator = allocators[listIndex];
		commandList = commandLists[listIndex];
	}
}