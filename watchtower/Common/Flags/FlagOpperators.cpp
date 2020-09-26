#include "FormatFlags.h"
#include "BindFlags.h"
#include "CPUAccessFlags.h"

#include <type_traits>

namespace Citadel::Watchtower::Flags {
	FormatFlags
	operator | (FormatFlags lhs, FormatFlags rhs) {
		using underlying = typename std::underlying_type<FormatFlags>::type;
		return static_cast<FormatFlags>(static_cast<underlying>(lhs) | static_cast<underlying>(rhs));
	}

	FormatFlags
	operator & (FormatFlags lhs, FormatFlags rhs) {
		using underlying = typename std::underlying_type<FormatFlags>::type;
		return static_cast<FormatFlags>(static_cast<underlying>(lhs) & static_cast<underlying>(rhs));
	}

	BindFlags
	operator | (BindFlags lhs, BindFlags rhs) {
		using underlying = typename std::underlying_type<BindFlags>::type;
		return static_cast<BindFlags>(static_cast<underlying>(lhs) | static_cast<underlying>(rhs));
	}

	BindFlags
	operator & (BindFlags lhs, BindFlags rhs) {
		using underlying = typename std::underlying_type<BindFlags>::type;
		return static_cast<BindFlags>(static_cast<underlying>(lhs) & static_cast<underlying>(rhs));
	}

	CPUAccessFlags
	operator | (CPUAccessFlags lhs, CPUAccessFlags rhs) {
		using underlying = typename std::underlying_type<CPUAccessFlags>::type;
		return static_cast<CPUAccessFlags>(static_cast<underlying>(lhs) | static_cast<underlying>(rhs));
	}

	CPUAccessFlags
	operator & (CPUAccessFlags lhs, CPUAccessFlags rhs) {
		using underlying = typename std::underlying_type<CPUAccessFlags>::type;
		return static_cast<CPUAccessFlags>(static_cast<underlying>(lhs) & static_cast<underlying>(rhs));
	}
}