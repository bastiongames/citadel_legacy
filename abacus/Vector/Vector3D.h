#ifndef __VECTOR3D_H__
#define __VECTOR3D_H__

#include "abacus/Point/Point3D.h"
using Citadel::Abacus::Point::Point3D;

namespace Citadel::Abacus::Vector {
	class Vector3D : protected Point3D
	{
	public:
		Vector3D();
		Vector3D(const real x, const real y, const real z);
		Vector3D(const Vector3D& other);
		Vector3D(Vector3D&& other);
		~Vector3D() = default;

		Vector3D& operator=(const Vector3D& other);
		Vector3D& operator=(const Vector3D&& other);
		bool operator==(const Vector3D& other) const;

		Vector3D operator-() const;
		
		Vector3D operator+(const Vector3D& other) const;
		void operator+=(const Vector3D& other);
		Vector3D operator-(const Vector3D& other) const;
		void operator-=(const Vector3D& other) ;
		Vector3D operator*(const real scalar) const;
		void operator*=(const real scalar);

		real X() const;
		real Y() const;
		real Z() const;

		real MagnitudeSquared() const;
		real Magnitude() const;
		real InverseMagnitude() const;

		void NormalizeSelf();
		Vector3D Normalized() const;;

		real DotProduct(const Vector3D& other) const;
		Vector3D CrossProduct(const Vector3D& other) const;

		Vector3D Project(const Vector3D& other) const;

		Vector3D Add(const Vector3D& other) const;
		Vector3D Subtract(const Vector3D& other) const;
		Vector3D Scale(const real scalar) const;
		Vector3D Negative() const;
	};
}

#endif;