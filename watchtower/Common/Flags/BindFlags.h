#ifndef __BINDFLAGS_H__
#define __BINDFLAGS_H__

namespace Citadel::Watchtower::Flags {
	enum struct BindFlags {
		None = 0,
		VertexBuffer = (1 << 0),
		IndexBuffer = (1 << 1),
		ConstantBuffer = (1 << 2),
		StreamOutputBuffer = (1 << 3),
		IndirectBuffer = (1 << 4),
		Sampled = (1 << 5),
		Storage = (1 << 6),
		ColorAttachment = (1 << 7),
		DepthStencilAttachment = (1 << 8),
		CombinedSampler = (1 << 9),
		CopySrc = (1 << 10),
		CopyDest = (1 << 11)
	};

	BindFlags operator | (BindFlags lhs, BindFlags rhs);
	BindFlags operator & (BindFlags lhs, BindFlags rhs);
}

#endif