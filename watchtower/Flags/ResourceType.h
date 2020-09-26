#ifndef __RESOURCETYPE_H__
#define __RESOURCETYPE_H__

namespace Citadel::Watchtower::Flags {
	enum struct ResourceType {
		Undefiend,
		Buffer,
		Texture,
		Sampler
	};

	ResourceType operator | (ResourceType lhs, ResourceType rhs);
	ResourceType operator & (ResourceType lhs, ResourceType rhs);
}

#endif