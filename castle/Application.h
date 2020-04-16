#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "Game.h"
namespace Citadel::Castle {
	class Application
	{
	public:
		static int Run(Game* game);
	private:
		static handle impl;
	};
}
#endif