#include "DualQuaternion.h"

using Citadel::Abacus::Quaternion::Quaternion;
using Citadel::Abacus::Quaternion::DualQuaternion;
using Citadel::Abacus::Vector::Vector3D;

DualQuaternion::DualQuaternion() {
	rQuaternion = Quaternion(0, 0, 0, 1);
	dQuaternion = Quaternion(0, 0, 0, 0);
}

DualQuaternion::DualQuaternion(const Quaternion& r, const Quaternion& d) {
	rQuaternion = r.Normalized();
	dQuaternion = d;
}

DualQuaternion::DualQuaternion(const Quaternion& r, const Vector3D& t) {
	rQuaternion = r.Normalized();
	dQuaternion = Quaternion(t, 0).CrossProduct(rQuaternion) * 0.5f;
}

DualQuaternion::DualQuaternion(const DualQuaternion& other) {
	rQuaternion = other.rQuaternion;
	dQuaternion = other.dQuaternion;
}

DualQuaternion::DualQuaternion(DualQuaternion&& other) {
	rQuaternion = other.rQuaternion;
	dQuaternion = other.dQuaternion;
}

DualQuaternion
DualQuaternion::Lerp(const DualQuaternion& other, real interp) const {
	return DualQuaternion(
		rQuaternion.Lerp(other.rQuaternion, interp),
		dQuaternion.Lerp(other.dQuaternion, interp)
	);
}

real
DualQuaternion::DotProduct() const {
	return rQuaternion.DotProduct(rQuaternion);
}

DualQuaternion
DualQuaternion::Add(const DualQuaternion& other) const {
	return DualQuaternion(
		rQuaternion + other.rQuaternion,
		dQuaternion + other.dQuaternion
	);
}

DualQuaternion
DualQuaternion::Scale(real scalar) const {
	return DualQuaternion(
		rQuaternion * scalar,
		dQuaternion * scalar
	);
}

DualQuaternion
DualQuaternion::Normalized() const {
	float invMag = rQuaternion.InverseMagnitude();
	return Scale(invMag);
}

void
DualQuaternion::NormalizeSelf() {
	rQuaternion.NormalizeSelf();
	dQuaternion.NormalizeSelf();
}

DualQuaternion
DualQuaternion::Conjugate() const {
	return DualQuaternion(
		rQuaternion.Conjugate(),
		dQuaternion.Conjugate()
	);
}

Vector3D
DualQuaternion::Translation() const {
	Quaternion t(dQuaternion * 2.0f);
	t = t.CrossProduct(rQuaternion.Conjugate());
	return Vector3D(t.X(), t.Y(), t.Z());
}

Quaternion DualQuaternion::Real() const { return rQuaternion; }
Quaternion DualQuaternion::Dual() const { return dQuaternion; }

