#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <map>
#include <fstream>
#include <sstream>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow* window);

unsigned int const SCR_WIDTH = 800;
unsigned int const SCR_HEIGHT = 600;

std::map<unsigned int, std::string> shaderType_to_shaderName = {
	{GL_VERTEX_SHADER,"VERTEX_SHADER"},
	{GL_FRAGMENT_SHADER,"FRAGMENT_SHADER"}
};

std::string readShader(const std::string& path) {
	std::ifstream ifs;
	ifs.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		ifs.open(path, std::ios::in);
		std::stringstream shaderStream;
		shaderStream << ifs.rdbuf();
		ifs.close();
		return shaderStream.str();
	}
	catch (std::ifstream::failure e) {
		std::cerr << "Failed in opening shader file: " << path << std::endl;
	}

	return "";
}


static unsigned int compileShader(unsigned int type, const std::string& source) {
	unsigned int shaderId = glCreateShader(type);
	const char* const src = source.c_str();
	glShaderSource(shaderId, 1, &src, nullptr);
	glCompileShader(shaderId);

	int result = 0;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length = 0;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(shaderId, length, nullptr, message);

		std::cout << "ERROR::SHADER:: " << shaderType_to_shaderName[type]
			<< " ::COMPILATION_FAILED\n" << message << std::endl;
		return 0;
	}

	return shaderId;
}

static unsigned int createShader(const std::string& vertexShaderSrc, const std::string& fragmentShaderSrc) {
	unsigned int shaderProgram = glCreateProgram();
	unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSrc);
	unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSrc);

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	GLint result = 0;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result);
	if (result == GL_FALSE) {
		char infoLog[512];
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glValidateProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

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

	float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
	};

	unsigned int VBO = 0;
	glGenBuffers(1, &VBO);
	unsigned int VAO = 0;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	
	std::cout << readShader("res/shaders/fragmentShader.shader");
	

	unsigned int shaderProgram = createShader(readShader("res/shaders/vertexShader.shader"),
		readShader("res/shaders/fragmentShader.shader"));

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);


		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(VAO);
		glUseProgram(shaderProgram);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwPollEvents();
		glfwSwapBuffers(window);

	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

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
}