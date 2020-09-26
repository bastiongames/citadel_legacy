#include "RenderContext.h"

using Citadel::Watchtower::Context::RenderContext;

RenderContext::RenderContext(handle data) {
	impl = data;
}

RenderContext::RenderContext(const RenderContext& context) {
	impl = context.impl;
}

RenderContext::RenderContext(RenderContext&& context) noexcept {
	this->impl = context.impl;
	context.impl = nullptr;
}

RenderContext::~RenderContext() noexcept {
	this->impl = nullptr;
}

RenderContext&
RenderContext::operator=(const RenderContext& context) {
	if (this != &context) {
		this->impl = context.impl;
	}
	return *this;
}

RenderContext&
RenderContext::operator=(RenderContext&& context) noexcept {
	if (this != &context) {
		this->impl = context.impl;
		context.impl = nullptr;
	}
	return *this;
}