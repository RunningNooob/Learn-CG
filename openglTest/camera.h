#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

glm::vec3 const POS = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 const FRONT = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 const UP = glm::vec3(0.0f, 1.0f, 0.0f);
float const YAW = -90.0f;
float const PITCH = 0.0f;
float const FOV = 45.0f;
float const SPEED = 2.5f;
float const SENSITIVITY = 0.05f;

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

class Camera {

	glm::vec3 pos;
	glm::vec3 front;
	glm::vec3 up;
	

	float yaw;
	float pitch;
	float fov;
	float speed;
	float sensitivity;

public:
	Camera(glm::vec3 pos = POS, glm::vec3 front = FRONT,glm::vec3 up = UP, 
		float yaw = YAW, float pitch = PITCH);

	float getFOV();

	glm::mat4 getViewMatrix();

	void processKeyBoard(Camera_Movement type, float deltaTime);

	void processMouseMovement(float xOffset, float yOffset);

	void processMouseScroll(float yoffset);
};