#ifndef __POINT3D_H__
#define __POINT3D_H__

#include "keep/Types.h"

using Citadel::Keep::real;

namespace Citadel::Abacus::Point {
	struct Point3D {
		Point3D() {
			x = y = z = 0.0f;
		}
		
		Point3D(const real x, const real y, const real z) {
			this->x = x;
			this->y = y;
		}

		Point3D(const Point3D& other) {
			this->x = other.x;
			this->y = other.y;
			this->z = other.z;
		}

		Point3D(Point3D&& other) {
			this->x = other.x;
			this->y = other.y;
			this->z = other.z;
		}

		Point3D operator=(const Point3D& other) {
			return Point3D(other);
		}

		Point3D operator=(Point3D&& other) {
			return Point3D(other.x, other.y, other.z);
		}

		real x;
		real y;
		real z;
	};
}

#endif