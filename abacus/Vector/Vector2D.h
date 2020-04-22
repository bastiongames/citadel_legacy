#ifndef __VECTOR2D_H__
#define __VECTOR2D_H__

#include "abacus/Point/Point2D.h"
using Citadel::Abacus::Point::Point2D;

namespace Citadel::Abacus::Vector {
	class Vector2D : protected Point2D
	{
	public:
		Vector2D();
		Vector2D(const real x, const real y);
		Vector2D(const Vector2D& other);
		Vector2D(Vector2D&& other);
		~Vector2D() = default;

		Vector2D& operator=(const Vector2D& other);
		Vector2D& operator=(const Vector2D&& other);
		bool operator==(const Vector2D& other) const;

		Vector2D operator-() const;
		
		Vector2D operator+(const Vector2D& other) const;
		void operator+=(const Vector2D& other);
		Vector2D operator-(const Vector2D& other) const;
		void operator-=(const Vector2D& other) ;
		Vector2D operator*(const real scalar) const;
		void operator*=(const real scalar);

		real X() const;
		real Y() const;

		real MagnitudeSquared() const;
		real Magnitude() const;
		real InverseMagnitude() const;

		void NormalizeSelf();
		Vector2D Normalized() const;;

		real DotProduct(const Vector2D& other) const;
		real CrossProduct(const Vector2D& other) const;

		Vector2D Project(const Vector2D& other) const;

		Vector2D Add(const Vector2D& other) const;
		Vector2D Subtract(const Vector2D& other) const;
		Vector2D Scale(const real scalar) const;
		Vector2D Negative() const;
	};
}

#endif