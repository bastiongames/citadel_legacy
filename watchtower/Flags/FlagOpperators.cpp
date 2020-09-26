#include "FormatFlags.h"
#include "BindFlags.h"
#include "CPUAccessFlags.h"
#include "ResourceType.h"
#include "StageFlags.h"

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

	StageFlags
	operator | (StageFlags lhs, StageFlags rhs) {
		using underlying = typename std::underlying_type<StageFlags>::type;
		return static_cast<StageFlags>(static_cast<underlying>(lhs) | static_cast<underlying>(rhs));
	}

	StageFlags
	operator & (StageFlags lhs, StageFlags rhs) {
		using underlying = typename std::underlying_type<StageFlags>::type;
		return static_cast<StageFlags>(static_cast<underlying>(lhs) & static_cast<underlying>(rhs));
	}

	ResourceType
	operator | (ResourceType lhs, ResourceType rhs) {
		using underlying = typename std::underlying_type<ResourceType>::type;
		return static_cast<ResourceType>(static_cast<underlying>(lhs) | static_cast<underlying>(rhs));
	}

	ResourceType
	operator & (ResourceType lhs, ResourceType rhs) {
		using underlying = typename std::underlying_type<ResourceType>::type;
		return static_cast<ResourceType>(static_cast<underlying>(lhs) & static_cast<underlying>(rhs));
	}
}