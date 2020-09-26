#ifndef __PIPELINELAYOUT_H__
#define __PIPELINELAYOUT_H__

#include "keep/Types.h"

namespace Citadel::Watchtower::Pipelines {
	class PipelineLayout {
	public:
		virtual ~PipelineLayout() = default;
	};

	using PPipelineLayout = Keep::SPtr<PipelineLayout>;
}

#endif