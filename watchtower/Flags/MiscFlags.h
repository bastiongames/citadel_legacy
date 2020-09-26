#ifndef __MISCFLAGS_H__
#define __MISCFLAGS_H__

namespace Citadel::Watchtower::Flags {
	enum struct MiscFlags {
		None = 0,
		DynamicUsage = (1 << 0),
		FixedSamples = (1 << 1),
		GenerateMips = (1 << 2),
		NoInitialData = (1 << 3),
		Append = (1 << 4),
		Counter = (1 << 5),
	};
}

#endif