#include "Matrix4x4.h"

using Citadel::Abacus::Matrix::Matrix4x4;

Matrix4x4::Matrix4x4() {
	memset(m, sizeof(real) * 16, 0);
}

Matrix4x4::Matrix4x4(const Matrix4x4& other) {
	memcpy(m, other.m, sizeof(real) * 16);
}

Matrix4x4::Matrix4x4(real m11, real m12, real m13, real m14, real m21, real m22, real m23, real m24, real m31, real m32, real m33, real m34, real m41, real m42, real m43, real m44) {
	this->m11 = m11; this->m12 = m12; this->m13 = m13; this->m14 = m14;
	this->m21 = m21; this->m22 = m22; this->m23 = m23; this->m24 = m24;
	this->m31 = m31; this->m32 = m32; this->m33 = m33; this->m34 = m34;
	this->m41 = m41; this->m42 = m42; this->m43 = m43; this->m44 = m44;
}

Matrix4x4
Matrix4x4::MakeZero() {
	return Matrix4x4();
}

Matrix4x4
Matrix4x4::MakeIdentity() {
	return Matrix4x4::MakeDiagonal(1, 1, 1, 1);
}

Matrix4x4
Matrix4x4::MakeDiagonal(real m11, real m22, real m33, real m44) {
	return Matrix4x4(
		m11, 0, 0, 0,
		0, m22, 0, 0,
		0, 0, m33, 0,
		0, 0, 0, m44
	);
}

Matrix4x4
Matrix4x4::Transposed() {
	return Matrix4x4(
		m11, m21, m31, m41,
		m12, m22, m32, m42,
		m13, m23, m33, m43,
		m14, m24, m34, m44
	);
}