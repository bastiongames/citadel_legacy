#ifndef __RESOURCEHEAP_H__
#define __RESOURCEHEAP_H__

#include "watchtower/Textures/Texture.h"
#include "watchtower/Buffers/Buffer.h"
#include "watchtower/Pipelines/PipelineLayout.h"

namespace Citadel::Watchtower {
	struct ResourceHeapDescriptor {
		Pipelines::PPipelineLayout pipelineLayout;
		Keep::u32 resourceCount = 0;
		Keep::u32 samplerCount = 0;
	};

	class ResourceHeap {
	public:
		virtual void BindTexture(const Textures::PTexture texture, Keep::u32 slot) = 0;
		virtual void BindBuffer(const Buffers::PBuffer buffer, Keep::u32 slot) = 0;
	};

	using PResourceHeap = Keep::SPtr<ResourceHeap>;
}

#endif