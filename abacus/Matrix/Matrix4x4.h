#ifndef __MATRIX4X4_H__
#define __MATRIX4X4_H__

#include "keep/Types.h"
using Citadel::Keep::real;

namespace Citadel::Abacus::Matrix {
	class Matrix4x4 {
	public:
		Matrix4x4();
		Matrix4x4(real m11, real m12, real m13, real m14, real m21, real m22, real m23, real m24, real m31, real m32, real m33, real m34, real m41, real m42, real m43, real m44);
		Matrix4x4(const Matrix4x4& other);
		Matrix4x4(Matrix4x4&& other);

		static Matrix4x4 MakeZero();
		static Matrix4x4 MakeIdentity();
		static Matrix4x4 MakeDiagonal(real m00, real m11, real m22, real m33 = 1);

		Matrix4x4 Transposed();
	
	private:
		union {
			real m[16];
			struct {
				real m11, m12, m13, m14;
				real m21, m22, m23, m24;
				real m31, m32, m33, m34;
				real m41, m42, m43, m44;
			};
		};
	};
}

#endif