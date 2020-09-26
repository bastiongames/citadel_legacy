#include "GraphicsPipeline.h"
#include "../d3dx12.h"

#include "../Shaders/ShaderProgram.h"
#include "PipelineLayout.h"

namespace Citadel::Watchtower::Pipelines::DX12 {
	GraphicsPipeline::GraphicsPipeline(ComPtr<ID3D12Device> device, const GraphicsPipelineDescriptor& descriptor) {
		auto shaderProgram = Keep::CastSPtr<Shaders::ShaderProgram, Shaders::DX12::ShaderProgram>(descriptor.shaderProgram);
		auto pipelineLayout = Keep::CastSPtr<Pipelines::PipelineLayout, PipelineLayout>(descriptor.pipelineLayout);

		rootSignature = pipelineLayout->GetRootSignature();
		{
			D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDescriptor{};
			pipelineDescriptor.InputLayout = shaderProgram->GetLayoutDescriptor();
			pipelineDescriptor.pRootSignature = pipelineLayout->GetRootSignature();
			pipelineDescriptor.VS = CD3DX12_SHADER_BYTECODE((ID3DBlob*)descriptor.shaderProgram->GetBytecode(Shaders::ShaderType::Vertex));
			pipelineDescriptor.PS = CD3DX12_SHADER_BYTECODE((ID3DBlob*)descriptor.shaderProgram->GetBytecode(Shaders::ShaderType::Pixel));
			pipelineDescriptor.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
			pipelineDescriptor.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
			pipelineDescriptor.DepthStencilState.DepthEnable = FALSE;
			pipelineDescriptor.DepthStencilState.StencilEnable = FALSE;
			pipelineDescriptor.SampleMask = UINT_MAX;
			pipelineDescriptor.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
			pipelineDescriptor.NumRenderTargets = 1;
			pipelineDescriptor.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
			pipelineDescriptor.SampleDesc.Count = 1;

			HRESULT hr = device->CreateGraphicsPipelineState(&pipelineDescriptor, IID_PPV_ARGS(&pipelineState));

			int a = 0; ++a;
		}
	}

	void
	GraphicsPipeline::Record(const ComPtr<ID3D12GraphicsCommandList> commandList) {
		commandList->SetGraphicsRootSignature(rootSignature.Get());
		commandList->SetPipelineState(pipelineState.Get());
	}
}