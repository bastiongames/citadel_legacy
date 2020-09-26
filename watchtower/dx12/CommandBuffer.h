#ifndef __DX12__COMMANDBUFFER_H__
#define __DX12__COMMANDBUFFER_H__

#include "watchtower/CommandBuffer.h"
#include "watchtower/RenderPass.h"

#include "RenderTarget.h"

#include <d3d12.h>
#include <wrl/client.h>
using Microsoft::WRL::ComPtr;

#include <vector>

namespace Citadel::Watchtower::DX12 {
	class CommandBuffer : public Watchtower::CommandBuffer
	{
	public:
		CommandBuffer(ComPtr<ID3D12Device> device);

		virtual void Begin();
		virtual void SetViewport(Keep::Frustum viewport, Keep::Rect scissor);
		virtual void SetPipeline(const Pipelines::PGraphicsPipeline pipeline);
		virtual void SetVertexBuffer(const Buffers::PBuffer vertexBuffer);
		virtual void SetIndexBuffer(const Buffers::PBuffer indexBuffer);
		virtual void BeginRenderPass(const PRenderPass renderPass);
		virtual void Clear(Keep::rgba clearColor);
		virtual void SetResourceHeap(const PResourceHeap resourceHeap);
		virtual void Draw(Keep::u32 count, Keep::u32 offset);
		virtual void DrawIndexed(Keep::u32 count, Keep::u32 offset);
		virtual void EndRenderPass();

		virtual void End();

	public:
		void Record(ComPtr<ID3D12CommandQueue> queue);

	private:
		void NextList();
	private:
		std::vector<ComPtr<ID3D12CommandAllocator>> allocators;
		ComPtr<ID3D12CommandAllocator> allocator;
		std::vector<ComPtr<ID3D12GraphicsCommandList>> commandLists;
		ComPtr<ID3D12GraphicsCommandList> commandList;
		
		std::vector<Keep::u64> fenceValues;
		ComPtr<ID3D12Fence> fence;
		HANDLE fenceEvent;

		Keep::u32 listIndex;


		Keep::SPtr<RenderTarget> renderTarget;
	};
}

#endif