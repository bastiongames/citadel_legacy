#include "Matrix4x4.h"

using Citadel::Abacus::Matrix::Matrix4x4;

Matrix4x4::Matrix4x4() {
	memset(m, sizeof(real) * 16, 0);
}

Matrix4x4::Matrix4x4(const Matrix4x4& other) {
	memcpy(m, other.m, sizeof(real) * 16);
}

Matrix4x4::Matrix4x4(Matrix4x4&& other) noexcept {
	memcpy(m, other.m, sizeof(real) * 16);
}


Matrix4x4::Matrix4x4(real m11, real m12, real m13, real m14, real m21, real m22, real m23, real m24, real m31, real m32, real m33, real m34, real m41, real m42, real m43, real m44) {
	this->m11 = m11; this->m12 = m12; this->m13 = m13; this->m14 = m14;
	this->m21 = m21; this->m22 = m22; this->m23 = m23; this->m24 = m24;
	this->m31 = m31; this->m32 = m32; this->m33 = m33; this->m34 = m34;
	this->m41 = m41; this->m42 = m42; this->m43 = m43; this->m44 = m44;
}

void
Matrix4x4::operator=(const Matrix4x4& other) {
	memcpy(m, other.m, sizeof(real) * 16);
}

Matrix4x4
Matrix4x4::operator*(const Matrix4x4& other) {
	return Matrix4x4(
		(m11 * other.m11) + (m12 * other.m21) + (m13 * other.m31) + (m14 * other.m41), (m11 * other.m12) + (m12 * other.m22) + (m13 * other.m32) + (m14 * other.m42), (m11 * other.m13) + (m12 * other.m23) + (m13 * other.m33) + (m14 * other.m43), (m11 * other.m14) + (m12 * other.m24) + (m13 * other.m34) + (m14 * other.m44),
		(m21 * other.m11) + (m22 * other.m21) + (m23 * other.m31) + (m24 * other.m41), (m21 * other.m12) + (m22 * other.m22) + (m23 * other.m32) + (m24 * other.m42), (m21 * other.m13) + (m22 * other.m23) + (m23 * other.m33) + (m24 * other.m43), (m21 * other.m14) + (m22 * other.m24) + (m23 * other.m34) + (m24 * other.m44),
		(m31 * other.m11) + (m32 * other.m21) + (m33 * other.m31) + (m34 * other.m41), (m31 * other.m12) + (m32 * other.m22) + (m33 * other.m32) + (m34 * other.m42), (m31 * other.m13) + (m32 * other.m23) + (m33 * other.m33) + (m34 * other.m43), (m31 * other.m14) + (m32 * other.m24) + (m33 * other.m34) + (m34 * other.m44),
		(m41 * other.m11) + (m42 * other.m21) + (m43 * other.m31) + (m44 * other.m41), (m41 * other.m12) + (m42 * other.m22) + (m43 * other.m32) + (m44 * other.m42), (m41 * other.m13) + (m42 * other.m23) + (m43 * other.m33) + (m44 * other.m43), (m41 * other.m14) + (m42 * other.m24) + (m43 * other.m34) + (m44 * other.m44)
	);
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