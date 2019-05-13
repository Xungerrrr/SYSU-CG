#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

// default camera values
const float YAW = 90.0f;
const float PITCH = -45.0f;
const float FOV = 45.0f;

class Camera {
public:
	// Constructor with vectors
	Camera(glm::vec3 _position = glm::vec3(0.0f, 0.0f, 15.0f), glm::vec3 _front = glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3 _up = glm::vec3(0.0f, 1.0f, 0.0f), float _yaw = YAW, float _pitch = PITCH);
	// Returns the view matrix
	glm::mat4 getViewMatrix();
	// Move the camera
	void moveForward(float distance);
	void moveBack(float distance);
	void moveRight(float distance);
	void moveLeft(float distance);
	// Rotate the camera using pitch and yaw
	void rotate(float _pitch, float _yaw);
	// Change the fov to zoom
	void zoom(float yoffset);
	// Get the fov
	float getFov();
	// Reset the camera
	void reset();
	glm::vec3 getPosition() { return position; }

private:
	// Calculates the front vector from the Camera's (updated) Euler Angles
	void updateCameraVectors();
	// Camera Attributes
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 world_up;
	// Euler Angles
	float yaw;
	float pitch;
	// Camera options
	float fov;
};
#endif