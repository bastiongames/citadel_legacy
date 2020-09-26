#ifndef __MATRIXHELPERS_H__
#define __MATRIXHELPERS_H__

#include "Matrix4x4.h"
#include "abacus/Vector/Vector3D.h"
#include "keep/Types.h"

namespace Citadel::Abacus::Matrix {
	Matrix4x4 RotationX(Keep::real theta);
	Matrix4x4 RotationY(Keep::real theta);
	Matrix4x4 RotationZ(Keep::real theta);
	Matrix4x4 Rotation(Keep::real x, Keep::real y, Keep::real z);


	Matrix4x4 LookAt_LH(const Vector::Vector3D& pos, const Vector::Vector3D& at, const Vector::Vector3D& up);
	Matrix4x4 LookAt_RH(const Vector::Vector3D& pos, const Vector::Vector3D& at, const Vector::Vector3D& up);
	
	Matrix4x4 PerspectiveFOV_LH(Keep::real fov, Keep::real aspectRatio, Keep::real n, Keep::real f);
	Matrix4x4 PerspectiveFOV_RH(Keep::real fov, Keep::real aspectRatio, Keep::real n, Keep::real f);

	Matrix4x4 OrthographicOffCenter_LH(Keep::real l, Keep::real r, Keep::real b, Keep::real t, Keep::real n, Keep::real f);
	Matrix4x4 OrthographicOffCenter_RH(Keep::real l, Keep::real r, Keep::real b, Keep::real t, Keep::real n, Keep::real f);

	Matrix4x4 Orthographic_LH(Keep::real w, Keep::real h, Keep::real n, Keep::real f);
	Matrix4x4 Orthographic_RH(Keep::real w, Keep::real h, Keep::real n, Keep::real f);

	typedef Matrix4x4 (*PLookAt)(const Vector::Vector3D&, const Vector::Vector3D&, const Vector::Vector3D&);
	typedef Matrix4x4 (*PPerspectiveFOV)(Keep::real, Keep::real, Keep::real, Keep::real);
	typedef Matrix4x4 (*POrthographicOffCenter)(Keep::real, Keep::real, Keep::real, Keep::real, Keep::real, Keep::real);
	typedef Matrix4x4 (*POrthographic)(Keep::real, Keep::real, Keep::real, Keep::real);

	extern PLookAt LookAt;
	extern PPerspectiveFOV PerspectiveFOV;
	extern POrthographicOffCenter OrthographicOffCenter;
	extern POrthographic Orthographic;
}

#endif