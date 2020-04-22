#ifndef __POINT2D_H__
#define __POINT2D_H__

#include "keep/Types.h"

using Citadel::Keep::real;

namespace Citadel::Abacus::Point {
	struct Point2D {
		Point2D() {
			x = y = 0.0f;
		}
		
		Point2D(const real x, const real y) {
			this->x = x;
			this->y = y;
		}

		Point2D(const Point2D& other) {
			this->x = other.x;
			this->y = other.y;
		}

		Point2D(Point2D&& other) {
			this->x = other.x;
			this->y = other.y;
		}

		Point2D operator=(const Point2D& other) {
			return Point2D(other);
		}

		Point2D operator=(Point2D&& other) {
			return Point2D(other.x, other.y);
		}

		real x;
		real y;
	};
}

#endif