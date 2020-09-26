#ifndef __DX12__GRAPHICSPIPELINE_H__
#define __DX12__GRAPHICSPIPELINE_H__

#include "watchtower/Pipelines/GraphicsPipeline.h"
#include "watchtower/Pipelines/GraphicsPipelineDescriptor.h"
#include "../Command.h"

#include <d3d12.h>
#include <wrl/client.h>
using Microsoft::WRL::ComPtr;

namespace Citadel::Watchtower::Pipelines::DX12 {
	class GraphicsPipeline : public Pipelines::GraphicsPipeline, public Command {
	public:
		GraphicsPipeline(ComPtr<ID3D12Device> device, const GraphicsPipelineDescriptor& descriptor);

		virtual void Record(const ComPtr<ID3D12GraphicsCommandList> commandList);
	private:
		ComPtr<ID3D12PipelineState> pipelineState;
		ComPtr<ID3D12RootSignature> rootSignature;
	};
}

#endif