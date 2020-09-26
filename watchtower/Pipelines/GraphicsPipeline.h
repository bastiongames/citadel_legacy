#ifndef __GRAPHICSPIPELINE_H__
#define __GRAPHICSPIPELINE_H__

#include "keep/Types.h"

namespace Citadel::Watchtower::Pipelines {
	class GraphicsPipeline {
	public:
		virtual ~GraphicsPipeline() = default;
	};

	using PGraphicsPipeline = Citadel::Keep::SPtr<GraphicsPipeline>;
}

#endif