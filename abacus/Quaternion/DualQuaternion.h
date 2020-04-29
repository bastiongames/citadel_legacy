#ifndef __DUALQUATERNION_H__
#define __DUALQUATERNION_H__

#include "keep/Types.h"
#include "abacus/Quaternion/Quaternion.h"
#include "abacus/Vector/Vector3D.h"

using Citadel::Abacus::Vector::Vector3D;

namespace Citadel::Abacus::Quaternion {
	class DualQuaternion {
	public:
		DualQuaternion();
		DualQuaternion(const Quaternion& r, const Quaternion& d);
		DualQuaternion(const Quaternion& r, const Vector3D& t);
		DualQuaternion(const DualQuaternion& other);
		DualQuaternion(DualQuaternion&& other);
		~DualQuaternion() = default;

		DualQuaternion& operator=(const DualQuaternion& other);
		DualQuaternion& operator=(const DualQuaternion&& other);
		bool operator==(const DualQuaternion& other) const;

		DualQuaternion Lerp(const DualQuaternion& other, real interp) const;
		real DotProduct() const;
		DualQuaternion Add(const DualQuaternion& other) const;
		DualQuaternion Scale(real scalar) const;
		DualQuaternion Normalized() const;
		void NormalizeSelf();

		DualQuaternion Conjugate() const;
		Vector3D Translation() const;

		Quaternion Real() const;
		Quaternion Dual() const;

	private:
		Quaternion rQuaternion;
		Quaternion dQuaternion;
	};
}

#endif
