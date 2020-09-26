#include "Camera.h"

using Citadel::Watchtower::Camera;

Camera::Camera(real focalLength, real frameHeight, real aspectRatio, real near, real far) {
	this->farPlane = far;
	this->nearPlane = near;
	this->aspectRatio = aspectRatio;
	SetFocalLength(focalLength, frameHeight);
}

void
Camera::SetFocalLength(real focalLength, real frameHeight) {
	this->focalLength = focalLength;
	this->frameHeight = frameHeight;
	
	this->fov = 2 * atanf(frameHeight / (focalLength * 2));
}

void
Camera::SetPosition(Vector3D pos) {
	position = pos;
}

void
Camera::LookAt(Vector3D pos) {
	target = pos;
}

real
Camera::AspectRatio() {
	return aspectRatio;
}

real
Camera::Far() {
	return farPlane;
}

real
Camera::FOV() {
	return fov;
}

real
Camera::Near() {
	return nearPlane;
}

Vector3D
Camera::GetPosition() {
	return position;
}

Vector3D
Camera::GetLookingAt() {
	return target;
}