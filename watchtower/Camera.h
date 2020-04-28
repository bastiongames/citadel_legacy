#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "keep/Types.h"
using Citadel::Keep::real;

#include "abacus/Vector/Vector3D.h"
using Citadel::Abacus::Vector::Vector3D;

namespace Citadel::Watchtower {
	class Camera {
	public:
		Camera(real focalLength = 50, real frameHeight = 24, real aspectRatio = 1.78f, real near = 0.01f, real far = 1000);

		void SetFocalLength(real focalLength, real frameHeight = 24);
		void SetPosition(Vector3D pos);
		void LookAt(Vector3D pos);

		real FOV();
		real AspectRatio();
		real Near();
		real Far();
		
		Vector3D GetPosition();
		Vector3D GetLookingAt();
	private:
		real frameHeight;
		real focalLength;
		real fov;
		real aspectRatio;
		real nearPlane;
		real farPlane;

		Vector3D position;
		Vector3D target;
	};
}

#endif