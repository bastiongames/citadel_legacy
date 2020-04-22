#include "Vector3D.h"

Citadel::Abacus::Vector::Vector3D::Vector3D() {
	this->x = this->y = this->z = 0.0f;
}

Citadel::Abacus::Vector::Vector3D::Vector3D(const real x, const real y, const real z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

Citadel::Abacus::Vector::Vector3D::Vector3D(const Vector3D& other) {
	this->x = other.x;
	this->y = other.y;
	this->z = other.z;
}

Citadel::Abacus::Vector::Vector3D::Vector3D(Vector3D&& other) {
	this->x = other.x;
	this->y = other.y;
	this->z = other.z;
}

Citadel::Abacus::Vector::Vector3D& 
Citadel::Abacus::Vector::Vector3D::operator=(const Vector3D& other) {
	this->x = other.x;
	this->y = other.y;
	this->z = other.z;
	return *this;
}

Citadel::Abacus::Vector::Vector3D&
Citadel::Abacus::Vector::Vector3D::operator=(const Vector3D&& other) {
	this->x = other.x;
	this->y = other.y;
	this->z = other.z;
	return *this;
}

bool
Citadel::Abacus::Vector::Vector3D::operator==(const Vector3D& other) const {
	return this->x == other.x && this->y == other.y && this->z == other.z;
}

Citadel::Abacus::Vector::Vector3D
Citadel::Abacus::Vector::Vector3D::operator-() const {
	return Vector3D(-this->x, -this->y, -this->z);
}

Citadel::Abacus::Vector::Vector3D
Citadel::Abacus::Vector::Vector3D::operator+(const Vector3D& other) const {
	return Vector3D(this->x + other.x, this->y + other.y, this->z + other.z);
}

void
Citadel::Abacus::Vector::Vector3D::operator+=(const Vector3D& other) {
	this->x += other.x;
	this->y += other.y;
	this->z += other.z;
}

Citadel::Abacus::Vector::Vector3D
Citadel::Abacus::Vector::Vector3D::operator-(const Vector3D& other) const {
	return Vector3D(this->x - other.x, this->y - other.y, this->z - other.z);
}

void
Citadel::Abacus::Vector::Vector3D::operator-=(const Vector3D& other) {
	this->x -= other.x;
	this->y -= other.y;
	this->z -= other.z;
}

Citadel::Abacus::Vector::Vector3D
Citadel::Abacus::Vector::Vector3D::operator*(const real scalar) const {
	return Vector3D(this->x * scalar, this->y * scalar, this->z * scalar);
}

void
Citadel::Abacus::Vector::Vector3D::operator*=(const real scalar) {
	this->x *= scalar;
	this->y *= scalar;
	this->z *= scalar;
}

real
Citadel::Abacus::Vector::Vector3D::X() const {
	return x;
}

real
Citadel::Abacus::Vector::Vector3D::Y() const {
	return y;
}

real
Citadel::Abacus::Vector::Vector3D::Z() const {
	return z;
}

real
Citadel::Abacus::Vector::Vector3D::MagnitudeSquared() const {
	return (this->x * this->x) + (this->y * this->y) + (this->z * this->z);
}

real
Citadel::Abacus::Vector::Vector3D::Magnitude() const {
	return sqrtf(this->MagnitudeSquared());
}

real
Citadel::Abacus::Vector::Vector3D::InverseMagnitude() const {
	return 1.0f / Magnitude();
}

void
Citadel::Abacus::Vector::Vector3D::NormalizeSelf() {
	real invMag = InverseMagnitude();
	this->x *= invMag;
	this->y *= invMag;
	this->z *= invMag;
}

Citadel::Abacus::Vector::Vector3D
Citadel::Abacus::Vector::Vector3D::Normalized() const {
	real invMag = InverseMagnitude();
	return this->Scale(invMag);
}

real
Citadel::Abacus::Vector::Vector3D::DotProduct(const Vector3D& other) const {
	return (this->x * other.x) + (this->y * other.y) + (this->z * other.z);
}

Citadel::Abacus::Vector::Vector3D
Citadel::Abacus::Vector::Vector3D::CrossProduct(const Vector3D& other) const {
	return Vector3D(
		(this->y * other.z) - (this->z * other.y),
		(this->z * other.x) - (this->x * other.z),
		(this->x * other.y) - (this->y * other.x)
	);
}

Citadel::Abacus::Vector::Vector3D
Citadel::Abacus::Vector::Vector3D::Project(const Vector3D& other) const {
	Vector3D result = other.Normalized();
	real dp = this->DotProduct(result);
	return result * dp;
}

Citadel::Abacus::Vector::Vector3D
Citadel::Abacus::Vector::Vector3D::Add(const Vector3D& other) const {
	return *this + other;
}

Citadel::Abacus::Vector::Vector3D
Citadel::Abacus::Vector::Vector3D::Subtract(const Vector3D& other) const {
	return *this - other;
}

Citadel::Abacus::Vector::Vector3D
Citadel::Abacus::Vector::Vector3D::Scale(const real scalar) const {
	return *this * scalar;
}

Citadel::Abacus::Vector::Vector3D
Citadel::Abacus::Vector::Vector3D::Negative() const {
	return -*this;
}