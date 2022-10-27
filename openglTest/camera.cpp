#include "camera.h"



Camera::Camera(glm::vec3 pos, glm::vec3 front, glm::vec3 up, float yaw , float pitch) {

	this->pos = pos;
	this->front = front;
	this->up = up;
	this->fov = FOV;
	this->speed = SPEED;
	this->sensitivity = SENSITIVITY;
	this->yaw = yaw;
	this->pitch = pitch;
}

float Camera::getFOV()
{
	return this->fov;
}

glm::mat4 Camera::getViewMatrix() {
	return glm::lookAt(pos, pos + front, up);
}

void Camera::processKeyBoard(Camera_Movement type, float deltaTime)
{
	float velocity = speed * deltaTime;
	
	switch (type) {
		case FORWARD:
			pos += velocity * front;
			break;

		case BACKWARD:
			pos -= velocity * front;
			break;

		case LEFT:
			pos -= velocity * glm::normalize(glm::cross(front, up));
			break;

		case RIGHT:
			pos += velocity * glm::normalize(glm::cross(front, up));
			break;

	}
}

void Camera::processMouseMovement(float xOffset, float yOffset)
{
	xOffset *= sensitivity;
	yOffset *= sensitivity;

	yaw += xOffset;
	pitch += yOffset;

	if (pitch > 89.0f) {
		pitch = 89.0f;
	}
	else if (pitch < -89.0f) {
		pitch = 89.0f;
	}

	front.x = glm::cos(glm::radians(pitch)) * glm::cos(glm::radians(yaw));
	front.y = glm::sin(glm::radians(pitch));
	front.z = glm::cos(glm::radians(pitch)) * glm::sin(glm::radians(yaw));
	front = glm::normalize(front);

	up.x = glm::sin(glm::radians(pitch)) * (-1.0) * glm::cos(glm::radians(yaw));
	up.y = glm::cos(glm::radians(pitch));
	up.z = glm::sin(glm::radians(pitch)) * (-1.0) * glm::sin(glm::radians(yaw));
	up = glm::normalize(up);
	
}

void Camera::processMouseScroll(float yOffset)
{
	float tmp = fov - yOffset;
	if (tmp < 1.0f) {
		fov = 1.0f;
	}
	else if (tmp > 80.0f) {
		fov = 80.0f;
	}
	else {
		fov = tmp;
	}
}
