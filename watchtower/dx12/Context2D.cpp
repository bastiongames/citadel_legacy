#include "Context.h"
using Citadel::Watchtower::DX12::ContextData;
using Citadel::Watchtower::Context;
using Citadel::Watchtower::Context2D;

#include "keep/Types.h"
using Citadel::Keep::SPtrFromHandle;

#include "watchtower/dx12/RenderTarget.h"
using Citadel::Watchtower::DX12::RenderTargetData;

#include "watchtower/dx12/Pipeline.h"
using Citadel::Watchtower::DX12::PipelineData;

#include "watchtower/dx12/Texture.h"
using Citadel::Watchtower::DX12::TextureData;

#include "watchtower/dx12/vertexbuffer.h"
using Citadel::Watchtower::DX12::VertexBufferData;

#include "d3dx12.h"

Context&
Context2D::Begin(RenderTarget& target, u32 frameIndex) {
	auto contextData = SPtrFromHandle<ContextData>(impl);
	auto targetData = SPtrFromHandle<RenderTargetData>(target.Get());
	auto frame = targetData->frames[frameIndex];
	HRESULT hr = S_OK;
	
	hr = contextData->allocators[frameIndex]->Reset();
	hr = contextData->commandList->Reset(contextData->allocators[frameIndex].Get(), nullptr);

	contextData->commandList->ResourceBarrier(
		1,
		&CD3DX12_RESOURCE_BARRIER::Transition(frame.target.Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET)
	);

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(
		targetData->heap->GetCPUDescriptorHandleForHeapStart(),
		frameIndex,
		targetData->size
	);

	const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
	contextData->commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);
	contextData->commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);

	return *this;
}

Context&
Context2D::UsingPipeline(Pipeline& pipeline) {
	auto contextData = SPtrFromHandle<ContextData>(impl);
	auto pipelineData = SPtrFromHandle<PipelineData>(pipeline.Get());

	contextData->commandList->SetPipelineState(pipelineData->pipelineState.Get());
	contextData->commandList->SetGraphicsRootSignature(pipelineData->signature.Get());

	contextData->commandList->RSSetViewports(1, &pipelineData->viewport);
	contextData->commandList->RSSetScissorRects(1, &pipelineData->scissorRect);

	return *this;
}

Context&
Context2D::PaintWith(Texture& texture) {
	auto contextData = SPtrFromHandle<ContextData>(impl);
	auto textureData = SPtrFromHandle<TextureData>(texture.Get());

	ID3D12DescriptorHeap* heaps[] = { textureData->heap.Get() };
	contextData->commandList->SetDescriptorHeaps(_countof(heaps), heaps);
	contextData->commandList->SetGraphicsRootDescriptorTable(0, textureData->heap->GetGPUDescriptorHandleForHeapStart());
	return *this;
}

Context&
Context2D::DrawTriangleList(VertexBuffer& vbuffer) {
	auto contextData = SPtrFromHandle<ContextData>(impl);
	auto vbufferData = SPtrFromHandle<VertexBufferData>(vbuffer.Get());

	contextData->commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	contextData->commandList->IASetVertexBuffers(0, 1, &vbufferData->view);
	contextData->commandList->DrawInstanced(3, 1, 0, 0);
	
	return *this;
}

Context&
Context2D::Finish(RenderTarget& target, u32 frameIndex) {
	auto contextData = SPtrFromHandle<ContextData>(impl);
	auto targetData = SPtrFromHandle<RenderTargetData>(target.Get());

	auto frame = targetData->frames[frameIndex];

	contextData->commandList->ResourceBarrier(
		1,
		&CD3DX12_RESOURCE_BARRIER::Transition(frame.target.Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT)
	);
	HRESULT hr = contextData->commandList->Close();


	return *this;
}