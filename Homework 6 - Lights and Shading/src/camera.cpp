#include "camera.h"

// Constructor with vectors
Camera::Camera(glm::vec3 _position, glm::vec3 _front, glm::vec3 _up, float _yaw, float _pitch) {
	position = _position;
	front = _front;
	world_up = _up;
	yaw = _yaw;
	pitch = _pitch;
	fov = FOV;
	updateCameraVectors();
}

// Returns the view matrix
glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(position, position + front, up);
}

// Move the camera
void Camera::moveForward(float distance) {
	position += front * distance;
}

void Camera::moveBack(float distance) {
	position -= front * distance;
}

void Camera::moveLeft(float distance) {
	position -= right * distance;
}

void Camera::moveRight(float distance) {
	position += right * distance;
}

// Rotate the camera using pitch and yaw
void Camera::rotate(float _pitch, float _yaw) {
	pitch += _pitch;
	yaw += _yaw;
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;
	updateCameraVectors();
}

// Change the fov to zoom
void Camera::zoom(float yoffset)
{
	if (fov >= 1.0f && fov <= 45.0f)
		fov -= yoffset;
	if (fov <= 1.0f)
		fov = 1.0f;
	if (fov >= 45.0f)
		fov = 45.0f;
}

// Get the fov
float Camera::getFov() {
	return glm::radians(fov);
}

// Reset the camera
void Camera::reset() {
	position = glm::vec3(0.0f, 0.0f, 15.0f);
	front = glm::vec3(0.0f, 0.0f, -1.0f);
	yaw = YAW;
	pitch = PITCH;
	fov = FOV;
	updateCameraVectors();
}

// Calculates the front vector from the Camera's (updated) Euler Angles
void Camera::updateCameraVectors()
{
	// Calculate the new Front vector
	glm::vec3 temp;
	temp.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	temp.y = sin(glm::radians(pitch));
	temp.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(temp);
	// Also re-calculate the Right and Up vector
	right = glm::normalize(glm::cross(front, world_up));  // Normalize the vectors
	up = glm::normalize(glm::cross(right, front));
}