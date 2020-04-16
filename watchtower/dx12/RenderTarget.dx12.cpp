#include "watchtower/RenderTarget.h"
#include "watchtower/dx12/RenderTarget.h"

using Citadel::Watchtower::DX12::RenderTargetData;

handle
Citadel::Watchtower::RenderTarget::GetFrame(u32 index) {
	auto data = Citadel::Keep::SPtrFromHandle<RenderTargetData>(impl);
	auto frame = data->renderTargets[index].Get();

	return std::make_shared<ComPtr<ID3D12Resource>>(frame);
}