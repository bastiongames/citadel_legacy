#include "Quaternion.h"

using Citadel::Abacus::Quaternion::Quaternion;

Quaternion::Quaternion() {
	x = y = z = 0.0f;
	r = 1.0f;
}

Quaternion::Quaternion(real x, real y, real z, real r) {
	this->x = x;
	this->y = y;
	this->z = z;
	this->r = r;
}

Quaternion::Quaternion(const Vector3D& v, real r) {
	this->x = v.X();
	this->y = v.Y();
	this->z = v.Z();
	this->r = r;
}

Quaternion::Quaternion(const Quaternion& other) {
	this->x = other.x;
	this->y = other.y;
	this->z = other.z;
	this->r = other.r;
}

Quaternion::Quaternion(Quaternion&& other) {
	this->x = other.x;
	this->y = other.y;
	this->z = other.z;
	this->r = other.r;
}

Quaternion&
Quaternion::operator=(const Quaternion& other) {
	x = other.x;
	y = other.y;
	z = other.z;
	r = other.r;

	return *this;
}

Quaternion&
Quaternion::operator=(Quaternion&& other) {
	x = other.x;
	y = other.y;
	z = other.z;
	r = other.r;

	return *this;
}

bool
Quaternion::operator==(const Quaternion& other) const {
	return x == other.x && y == other.y && z == other.z && r == other.r;
}

Quaternion
Quaternion::Lerp(const Quaternion& other, real interp) const {
	return Scale(1.0f - interp) + other.Scale(interp);
}

Quaternion
Quaternion::Add(const Quaternion& other) const {
	return *this + other;
}

Quaternion
Quaternion::Subtract(const Quaternion& other) const {
	return *this - other;
}

real
Quaternion::DotProduct(const Quaternion& other) const {
	return (x * other.x) + (y * other.y) + (z * other.z) + (r * other.r);
}

Quaternion
Quaternion::CrossProduct(const Quaternion& other) const {
	Vector3D vA(x, y, z);
	Vector3D vB(other.x, other.y, other.z);

	real dp = vA.DotProduct(vB);
	Vector3D vec = (vA * other.r) + (vB * r) + vA.CrossProduct(vB);
	return Quaternion(
		vec.X(),
		vec.Y(),
		vec.Z(),
		(r * other.r) - dp
	);
}

Quaternion
Quaternion::Normalized() const {
	real invMag = InverseMagnitude();
	return Scale(invMag);
}

void
Quaternion::NormalizeSelf() {
	real invMag = InverseMagnitude();
	x *= invMag;
	y *= invMag;
	z *= invMag;
	r *= invMag;
}

Quaternion
Quaternion::Scale(real scalar) const {
	return *this * scalar;
}

Quaternion
Quaternion::Negative() const {
	return -(*this);
}

Quaternion
Quaternion::Inverse() const {
	real invMagSquared = 1.0f / MagnitudeSquared();
	return Conjugate() * invMagSquared;
}

Quaternion
Quaternion::Conjugate() const {
	return Quaternion(-x, -y, -z, r);
}

real
Quaternion::MagnitudeSquared() const {
	return (x * x) + (y * y) + (z * z) + (r * r);
}

real
Quaternion::Magnitude() const {
	return sqrtf(MagnitudeSquared());
}

real
Quaternion::InverseMagnitude() const {
	return 1.0f / Magnitude();
}

Vector3D
Quaternion::VectorTerm() const {
	return Vector3D(x, y, z);
}

real
Quaternion::ScalarTerm() const {
	return r;
}

real Quaternion::X() const { return x; }
real Quaternion::Y() const { return y; }
real Quaternion::Z() const { return z; }
real Quaternion::R() const { return r; }

Quaternion
Quaternion::operator-() const {
	return Quaternion(
		-x,
		-y,
		-z,
		-r
	);
}

Quaternion
Quaternion::operator+(const Quaternion& other) const {
	return Quaternion(
		x + other.x,
		y + other.y,
		z + other.z,
		r + other.r
	);
}

void
Quaternion::operator+=(const Quaternion& other) {
	x += other.x;
	y += other.y;
	z += other.z;
	r += other.r;
}

Quaternion
Quaternion::operator-(const Quaternion& other) const {
	return Quaternion(
		x - other.x,
		y - other.y,
		z - other.z,
		r - other.r
	);
}

void
Quaternion::operator-=(const Quaternion& other) {
	x -= other.x;
	y -= other.y;
	z -= other.z;
	r -= other.r;
}

Quaternion
Quaternion::operator*(real scalar) const {
	return Quaternion(
		x * scalar,
		y * scalar,
		z * scalar,
		r * scalar
	);
}

void
Quaternion::operator*=(real scalar) {
	x *= scalar;
	y *= scalar;
	z *= scalar;
	r *= scalar;
}

