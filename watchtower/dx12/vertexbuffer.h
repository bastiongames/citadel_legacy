#ifndef __DX12_VERTEXBUFFER_H__
#define __DX12_VERTEXBUFFER_H__

#include <d3d12.h>
#include "../VertexBuffer.h"

namespace Citadel::Watchtower::DX12 {
	struct VertexBufferData {
		ComPtr<ID3D12Resource> vbuffer;
		D3D12_VERTEX_BUFFER_VIEW view;
	};
}
#endif