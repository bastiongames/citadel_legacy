#ifndef __QUATERNION_H__
#define __QUATERNION_H__

#include "keep/Types.h"
#include "abacus/Vector/Vector3D.h"

using Citadel::Keep::real;
using Citadel::Abacus::Vector::Vector3D;

namespace Citadel::Abacus::Quaternion {
	class Quaternion {
	public:
		Quaternion();
		Quaternion(real x, real y, real z, real r);
		Quaternion(const Vector3D& v, real r);
		Quaternion(const Quaternion& other);
		Quaternion(Quaternion&& other);
		~Quaternion() = default;

		Quaternion& operator=(const Quaternion& other);
		Quaternion& operator=(Quaternion&& other);
		bool operator==(const Quaternion& other) const;

		Quaternion Lerp(const Quaternion& other, real interp) const;
		Quaternion Add(const Quaternion& other) const;
		Quaternion Subtract(const Quaternion& other) const;
		real DotProduct(const Quaternion& other) const;
		Quaternion CrossProduct(const Quaternion& other) const;
		Quaternion Normalized() const;
		void NormalizeSelf();
		Quaternion Scale(real scalar) const;
		Quaternion Negative() const;
		Quaternion Inverse() const;
		Quaternion Conjugate() const;
		real MagnitudeSquared() const;
		real Magnitude() const;
		real InverseMagnitude() const;

		Vector3D VectorTerm() const;
		real ScalarTerm() const;

		real X() const;
		real Y() const;
		real Z() const;
		real R() const;

		Quaternion operator-() const;
		Quaternion operator+(const Quaternion& other) const;
		void operator+=(const Quaternion& other);
		Quaternion operator-(const Quaternion& other) const;
		void operator-=(const Quaternion& other);
		Quaternion operator*(real scalar) const;
		void operator*=(real scalar);
	private:
		real x;
		real y;
		real z;
		real r;
	};
}
#endif