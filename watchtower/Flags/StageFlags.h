#ifndef __STAGEFLAGS_H__
#define __STAGEFLAGS_H__

namespace Citadel::Watchtower::Flags {
	enum struct StageFlags {
		Undefined = 0,

		VertexStage = (1 << 0),
		HullStage = (1 << 1),
		DomainStage = (1 << 2),
		GeometryStage = (1 << 3),
		PixelStage = (1 << 4),
		ComputeStage = (1 << 5),

		TessControlStage = HullStage,
		TessEvaluationStage = DomainStage,
		FragmentStage = PixelStage,

		AllTessStages = (TessControlStage | TessEvaluationStage),
		AllGraphicsStages = (VertexStage | AllTessStages | GeometryStage | FragmentStage),
		AllStages = (AllGraphicsStages | ComputeStage)
	};

	StageFlags operator | (StageFlags lhs, StageFlags rhs);
	StageFlags operator & (StageFlags lhs, StageFlags rhs);
}

#endif