#ifndef __SYSTEMVALUE_H__
#define __SYSTEMVALUE_H__

namespace Citadel::Watchtower {
	enum struct SystemValue {
		Undefined,
		ClipDistance,
		Color,
		CullDistance,
		Depth,
		DepthGreater,
		DepthLess,
		FrontFacing,
		InstanceID,
		Position,
		PrimativeID,
		RenderTargetIndex,
		SampleMask,
		SampleID,
		Stencil,
		VertexID,
		ViewportIndex,
	};
}

#endif