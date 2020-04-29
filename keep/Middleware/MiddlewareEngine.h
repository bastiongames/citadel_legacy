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

namespace Citadel::Keep::Middlware {
	class IMiddleware {
	public:
		virtual Callback Method() = 0;
	};

	using MiddlewareSet = std::vector<SPtr<IMiddleware>>;

	class MiddlewareEngine {
		void RegisterMiddleware(SPtr<IMiddleware> m) {
			middleware.push_back(m);
		}

		void Execute(Delegate<void()> method) {
			auto iterator = MakeSPtr<MiddlewareSet::iterator>(middleware.begin());
			auto next = MakeSPtr<next_t>();
			auto weakNext = std::weak_ptr<next_t>(next);

			*next = [this, method, iterator, weakNext] {
				auto& it = *iterator;
				if (it != middleware.end()) {
					auto func = (*it)->Method();
					++it;
					auto nxt = weakNext.lock();
					func(nxt);
				}
				else {
					method();
				}
			};
			(*next)();
		}
	private:
		MiddlewareSet middleware;
	};
}
#endif
