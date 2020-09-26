#ifndef __PIPELINESTATE_H__
#define __PIPELINESTATE_H__

#include "keep/Types.h"
using Citadel::Keep::handle;

namespace Citadel::Watchtower::Pipeline {
	class PipelineState {
	public:
		PipelineState(handle data = nullptr);
		PipelineState(const PipelineState& other);
		PipelineState(PipelineState&& other) noexcept;
		~PipelineState() noexcept;

		PipelineState& operator=(const PipelineState& other);
		PipelineState& operator=(PipelineState&& other) noexcept;

		handle Get();

	private:
		handle impl;
	};
}

#endif