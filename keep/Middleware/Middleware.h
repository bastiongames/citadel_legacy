#ifndef __MIDDLEWARE_H__
#define __MIDDLEWARE_H__

#include <functional>
#include <memory>

namespace Citadel::Keep::Middleware {
	template<class func>
	using Delegate = std::function<func>;

	using next_t = Delegate<void()>;
	using Next = std::shared_ptr<next_t>;

	using Callback = Delegate<void(Next)>;

	template<class F, class I>
	Callback BindCallback(F func, I inst) {
		return std::bind(func, inst, std::placeholders::_1);
	}

	template <class TContext>
	class IMiddleware {
	public:
		virtual void Execute(Next next, TContext context) = 0;
	};
}

#endif