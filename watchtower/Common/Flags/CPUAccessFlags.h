#ifndef __CPUACCESSFLAGS_H__
#define __CPUACCESSFLAGS_H__

namespace Citadel::Watchtower::Flags {
	enum struct CPUAccessFlags {
		None = 0,
		Read = (1 << 0),
		Write = (1 << 1),
		ReadWrite = (Read | Write)
	};

	CPUAccessFlags operator | (CPUAccessFlags lhs, CPUAccessFlags rhs);
	CPUAccessFlags operator & (CPUAccessFlags lhs, CPUAccessFlags rhs);
}

#endif