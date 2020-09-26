#ifndef __MIDDLEWAREENGINE_H__
#define __MIDDLEWAREENGINE_H__

#include "../Types.h"
#include "Middleware.h"
#include <vector>

using std::vector;
using Citadel::Keep::SPtr;
using Citadel::Keep::Middleware::Callback;
using Citadel::Keep::Middleware::Delegate;
using Citadel::Keep::Middleware::next_t;
using Citadel::Keep::Middleware::Next;

namespace Citadel::Keep::Middleware {

	template <class TContext>
	using MiddlewareSet = std::vector<SPtr<IMiddleware<TContext>>>;

	template <class TContext>
	class MiddlewareEngine {
	public:
		void RegisterMiddleware(SPtr<IMiddleware<TContext>> m) {
			middleware.push_back(m);
		}

		void Execute(Delegate<void(TContext)> method, TContext context) {
			auto iterator = MakeSPtr<MiddlewareSet<TContext>::iterator>(middleware.begin());
			auto next = MakeSPtr<next_t>();
			auto weakNext = std::weak_ptr<next_t>(next);

			*next = [this, method, iterator, weakNext, context] {
				auto& it = *iterator;
				if (it != middleware.end()) {
					auto current = (*it);
					++it;
					auto next = weakNext.lock();
					current->Execute(next, context);
				}
				else {
					method(context);
				}
			};
			(*next)();
		}
	private:
		MiddlewareSet<TContext> middleware;
	};
}
#endif
