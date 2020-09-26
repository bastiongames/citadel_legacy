#ifndef __PIPELINELAYOUTDESCRIPTOR_H__
#define __PIPELINELAYOUTDESCRIPTOR_H__

#include "watchtower/Flags/ResourceType.h"
#include "watchtower/Flags/StageFlags.h"
#include "watchtower/Flags/BindFlags.h"
#include "keep/Types.h"

#include <vector>
#include <string>

namespace Citadel::Watchtower::Pipelines {
	struct BindingDescriptor {
		inline BindingDescriptor() = default;
		inline BindingDescriptor(const BindingDescriptor&) = default;

		inline BindingDescriptor(Flags::ResourceType type, Flags::BindFlags bindFlags, Flags::StageFlags stageFlags, Keep::u32 slot, Keep::u32 arraySize = 1) :
			type{ type }, bindFlags{ bindFlags }, stageFlags{ stageFlags }, slot{ slot }, arraySize{ arraySize } {}

		inline BindingDescriptor(std::string name, Flags::ResourceType type, Flags::BindFlags bindFlags, Flags::StageFlags stageFlags, Keep::u32 slot, Keep::u32 arraySize = 1) :
			name{ name }, type { type }, bindFlags{ bindFlags }, stageFlags{ stageFlags }, slot{ slot }, arraySize{ arraySize } {}

		std::string name;
		Flags::ResourceType type = Flags::ResourceType::Undefiend;
		Flags::BindFlags bindFlags = Flags::BindFlags::None;
		Flags::StageFlags stageFlags = Flags::StageFlags::Undefined;
		Keep::u32 slot = 0;
		Keep::u32 arraySize = 1;
	};

	struct PipelineLayoutDescriptor {
		std::vector<BindingDescriptor> bindings;
	};
}

#endif