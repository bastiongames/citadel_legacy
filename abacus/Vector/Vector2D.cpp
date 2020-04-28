#include "Vector2D.h"

Citadel::Abacus::Vector::Vector2D::Vector2D() {
	this->x = this->y = 0.0f;
}

Citadel::Abacus::Vector::Vector2D::Vector2D(const real x, const real y) {
	this->x = x;
	this->y = y;
}

Citadel::Abacus::Vector::Vector2D::Vector2D(const Vector2D& other) {
	this->x = other.x;
	this->y = other.y;
}

Citadel::Abacus::Vector::Vector2D::Vector2D(Vector2D&& other) {
	this->x = other.x;
	this->y = other.y;
}

Citadel::Abacus::Vector::Vector2D& 
Citadel::Abacus::Vector::Vector2D::operator=(const Vector2D& other) {
	this->x = other.x;
	this->y = other.y;
	return *this;
}

Citadel::Abacus::Vector::Vector2D&
Citadel::Abacus::Vector::Vector2D::operator=(const Vector2D&& other) {
	this->x = other.x;
	this->y = other.y;
	return *this;
}

bool
Citadel::Abacus::Vector::Vector2D::operator==(const Vector2D& other) const {
	return this->x == other.x && this->y == other.y;
}

Citadel::Abacus::Vector::Vector2D
Citadel::Abacus::Vector::Vector2D::operator-() const {
	return Vector2D(-this->x, -this->y);
}

Citadel::Abacus::Vector::Vector2D
Citadel::Abacus::Vector::Vector2D::operator+(const Vector2D& other) const {
	return Vector2D(this->x + other.x, this->y + other.y);
}

void
Citadel::Abacus::Vector::Vector2D::operator+=(const Vector2D& other) {
	this->x += other.x;
	this->y += other.y;
}

Citadel::Abacus::Vector::Vector2D
Citadel::Abacus::Vector::Vector2D::operator-(const Vector2D& other) const {
	return Vector2D(this->x - other.x, this->y - other.y);
}

void
Citadel::Abacus::Vector::Vector2D::operator-=(const Vector2D& other) {
	this->x -= other.x;
	this->y -= other.y;
}

Citadel::Abacus::Vector::Vector2D
Citadel::Abacus::Vector::Vector2D::operator*(const real scalar) const {
	return Vector2D(this->x * scalar, this->y * scalar);
}

void
Citadel::Abacus::Vector::Vector2D::operator*=(const real scalar) {
	this->x *= scalar;
	this->y *= scalar;
}

real
Citadel::Abacus::Vector::Vector2D::X() const {
	return x;
}

real
Citadel::Abacus::Vector::Vector2D::Y() const {
	return y;
}

real
Citadel::Abacus::Vector::Vector2D::MagnitudeSquared() const {
	return (this->x * this->x) + (this->y * this->y);
}

real
Citadel::Abacus::Vector::Vector2D::Magnitude() const {
	return sqrtf(this->MagnitudeSquared());
}

real
Citadel::Abacus::Vector::Vector2D::InverseMagnitude() const {
	return 1.0f / Magnitude();
}

void
Citadel::Abacus::Vector::Vector2D::NormalizeSelf() {
	real invMag = InverseMagnitude();
	this->x *= invMag;
	this->y *= invMag;
}

Citadel::Abacus::Vector::Vector2D
Citadel::Abacus::Vector::Vector2D::Normalized() const {
	real invMag = InverseMagnitude();
	return this->Scale(invMag);
}

real
Citadel::Abacus::Vector::Vector2D::DotProduct(const Vector2D& other) const {
	return (this->x * other.x) + (this->y * other.y);
}

real
Citadel::Abacus::Vector::Vector2D::CrossProduct(const Vector2D& other) const {
	return (this->x * other.y) - (this->x * other.y);
}

Citadel::Abacus::Vector::Vector2D
Citadel::Abacus::Vector::Vector2D::Project(const Vector2D& other) const {
	Vector2D result = other.Normalized();
	real dp = this->DotProduct(result);
	return result * dp;
}

Citadel::Abacus::Vector::Vector2D
Citadel::Abacus::Vector::Vector2D::Add(const Vector2D& other) const {
	return *this + other;
}

Citadel::Abacus::Vector::Vector2D
Citadel::Abacus::Vector::Vector2D::Subtract(const Vector2D& other) const {
	return *this - other;
}

Citadel::Abacus::Vector::Vector2D
Citadel::Abacus::Vector::Vector2D::Scale(const real scalar) const {
	return *this * scalar;
}

Citadel::Abacus::Vector::Vector2D
Citadel::Abacus::Vector::Vector2D::Negative() const {
	return -*this;
}