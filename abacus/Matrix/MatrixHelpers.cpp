#include "MatrixHelpers.h"

namespace Citadel::Abacus::Matrix {
	Matrix4x4
	RotationX(Keep::real theta) {
		Keep::real a = cos(theta);
		Keep::real b = sin(theta);

		return Matrix4x4(
			1, 0, 0, 0,
			0, a, b, 0,
			0, -b, a, 0,
			0, 0, 0, 1
		);
	}

	Matrix4x4
	RotationY(Keep::real theta) {
		Keep::real a = cos(theta);
		Keep::real b = sin(theta);

		return Matrix4x4(
			a, 0, -b, 0,
			0, 1, 0, 0,
			b, 0, a, 0,
			0, 0, 0, 1
		);
	}
	
	Matrix4x4
	RotationZ(Keep::real theta) {
		Keep::real a = cos(theta);
		Keep::real b = sin(theta);

		return Matrix4x4(
			a, b, 0, 0,
			-b, a, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		);
	}

	Matrix4x4
	Rotation(Keep::real x, Keep::real y, Keep::real z) {
		return RotationZ(z) * RotationY(y) * RotationX(x);
	}


	Matrix4x4
	LookAt_LH(const Vector::Vector3D& pos, const Vector::Vector3D& at, const Vector::Vector3D& up) {
		Vector::Vector3D z = at.Subtract(pos).Normalized();
		Vector::Vector3D x = up.CrossProduct(z).Normalized();
		Vector::Vector3D y = z.CrossProduct(x).Normalized();

		return Matrix4x4(
			x.X(), y.X(), z.X(), 0,
			x.Y(), y.Y(), z.Y(), 0,
			x.Z(), y.Z(), z.Z(), 0,
			-x.DotProduct(pos), -y.DotProduct(pos), -z.DotProduct(pos), 1
		);
	}
	
	Matrix4x4
	LookAt_RH(const Vector::Vector3D& pos, const Vector::Vector3D& at, const Vector::Vector3D& up) {
		Vector::Vector3D z = pos.Subtract(at).Normalized();
		Vector::Vector3D x = up.CrossProduct(z).Normalized();
		Vector::Vector3D y = z.CrossProduct(x).Normalized();

		// Left Hand
		return Matrix4x4(
			x.X(), y.X(), z.X(), 0,
			x.Y(), y.Y(), z.Y(), 0,
			x.Z(), y.Z(), z.Z(), 0,
			-x.DotProduct(pos), -y.DotProduct(pos), -z.DotProduct(pos), 1
		);
	}

	Matrix4x4
	PerspectiveFOV_LH(Keep::real fov, Keep::real aspectRatio, Keep::real n, Keep::real f) {
		real y = (real)(1.0f / tan(fov / 2.0f));
		real x = y / aspectRatio;

		return Matrix4x4(
			x, 0, 0, 0,
			0, y, 0, 0,
			0, 0, (f / (f - n)), 1,
			0, 0, -n * f / (f - n), 0
		);
	}

	Matrix4x4
	PerspectiveFOV_RH(Keep::real fov, Keep::real aspectRatio, Keep::real n, Keep::real f) {
		real y = (real)(1.0f / tan(fov / 2.0f));
		real x = y / aspectRatio;

		return Matrix4x4(
			x, 0, 0, 0,
			0, y, 0, 0,
			0, 0, f / (n - f), -1,
			0, 0, n * f / (n - f), 0
		);
	}

	Matrix4x4
	OrthographicOffCenter_LH(Keep::real l, Keep::real r, Keep::real b, Keep::real t, Keep::real n, Keep::real f) {
		return Matrix4x4(
			2/(r - l), 0, 0, 0,
			0, 2/(t - b), 0, 0,
			0, 0, 1/(f - n), 0,
			(l + r)/(l - r), (t + b)/(b - t), n/(n - f), 1
		);
	}
	
	Matrix4x4
	OrthographicOffCenter_RH(Keep::real l, Keep::real r, Keep::real b, Keep::real t, Keep::real n, Keep::real f) {
		return Matrix4x4(
			2 / (r - l), 0, 0, 0,
			0, 2 / (t - b), 0, 0,
			0, 0, 1 / (n - f), 0,
			(l + r) / (l - r), (t + b) / (b - t), n / (n - f), 1
		);
	}

	Matrix4x4
	Orthographic_LH(Keep::real w, Keep::real h, Keep::real n, Keep::real f) {
		return Matrix4x4(
			2 / w, 0, 0, 0,
			0, 2 / h, 0, 0,
			0, 0, 1/(f-n), 0,
			0, 0, n/(n-f), 1
		);
	}

	Matrix4x4
	Orthographic_RH(Keep::real w, Keep::real h, Keep::real n, Keep::real f) {
		return Matrix4x4(
			2 / w, 0, 0, 0,
			0, 2 / h, 0, 0,
			0, 0, 1 / (n - f), 0,
			0, 0, n / (n - f), 1
		);
	}

	PLookAt LookAt = LookAt_LH;
	PPerspectiveFOV PerspectiveFOV = PerspectiveFOV_LH;
	POrthographicOffCenter OrthographicOffCenter = OrthographicOffCenter_LH;
	POrthographic Orthographic = Orthographic_LH;
}

