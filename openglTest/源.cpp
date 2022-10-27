#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <map>
#include <fstream>
#include <sstream>
#include "myShader.h"
#include "camera.h"

#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

unsigned int const SCR_WIDTH = 800;
unsigned int const SCR_HEIGHT = 600;

Camera camera;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float cursorLastX = SCR_WIDTH / 2;
float cursorLastY = SCR_HEIGHT / 2;


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
unsigned int createTexture(const char* path, unsigned int type);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

int main() {
	
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glEnable(GL_DEPTH_TEST);

	float vertices[] = {

		//顶点坐标				纹理坐标
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	unsigned int indices[]{
		0,1,2,
		0,3,2,
	};

	glm::vec3 cubePositions[10] = {
		  glm::vec3(0.0f,  0.0f,  0.0f),
		  glm::vec3(2.0f,  5.0f, -15.0f),
		  glm::vec3(-1.5f, -2.2f, -2.5f),
		  glm::vec3(-3.8f, -2.0f, -12.3f),
		  glm::vec3(2.4f, -0.4f, -3.5f),
		  glm::vec3(-1.7f,  3.0f, -7.5f),
		  glm::vec3(1.3f, -2.0f, -2.5f),
		  glm::vec3(1.5f,  2.0f, -2.5f),
		  glm::vec3(1.5f,  0.2f, -1.5f),
		  glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	
	unsigned int VAO = 0;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO = 0;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	/*unsigned int EBO = 0;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/

	//EBO解绑必须在VAO解绑之后
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	MyShader shader("res/shaders/vertexShader.vert", "res/shaders/fragmentShader.frag");

	unsigned int texture1 = createTexture("res/texture/container.jpg", GL_RGB);
	unsigned int texture2 = createTexture("res/texture/awesomeface.png", GL_RGBA);
	shader.use();
	shader.setInt("texture1", 0);
	shader.setInt("texture2", 1);


	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		shader.use();

		glm::mat4 projection(1.0f);
		projection = glm::perspective(glm::radians(camera.getFOV()), (float)(SCR_WIDTH / SCR_HEIGHT), 0.1f, 100.0f);

		glm::mat4 view = camera.getViewMatrix();
		shader.setMatrix4fv("view", glm::value_ptr(view));
		shader.setMatrix4fv("projection", glm::value_ptr(projection));


		glBindVertexArray(VAO);
		//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);


		for (int i = 0; i < 10; ++i) {
			glm::mat4 model(1.0f);
			model = glm::translate(model, cubePositions[i]);
			model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f) * (i), glm::vec3(0.5f, 1.0f, 0.0f));

			shader.setMatrix4fv("model", glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glBindVertexArray(0);


		glfwPollEvents();
		glfwSwapBuffers(window);

	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	shader.delProgram();

	glfwTerminate();

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.processKeyBoard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.processKeyBoard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.processKeyBoard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.processKeyBoard(RIGHT, deltaTime);
}

unsigned int createTexture(const char* path, unsigned int type) {
	unsigned int texture = 0;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannerls;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(path, &width, &height, &nrChannerls, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, type, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture: " << path << std::endl;
	}

	stbi_image_free(data);

	return texture;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos){

	float offsetX = xpos - cursorLastX;
	float offsetY = cursorLastY - ypos;
	cursorLastX = xpos;
	cursorLastY = ypos;

	camera.processMouseMovement(offsetX, offsetY);
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset){
	camera.processMouseScroll(yOffset);
}
