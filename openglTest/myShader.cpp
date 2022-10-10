#include "myShader.h"


MyShader::MyShader(const std::string& vertaxShaderPath, const std::string& fragmentShaderPath) {

	programId = createShader(
		readShader(vertaxShaderPath),
		readShader(fragmentShaderPath));
}

std::string MyShader::readShader(const std::string& path) {
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


unsigned int MyShader::compileShader(unsigned int type, const std::string& source) {
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

		std::cout << "ERROR::SHADER:: " << shaderType_to_shaderNameStr[type]
			<< " ::COMPILATION_FAILED\n" << message << std::endl;
		return 0;
	}

	return shaderId;
}


unsigned int MyShader::createShader(const std::string& vertexShaderSrc, const std::string& fragmentShaderSrc) {
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

void MyShader::use() {
	glUseProgram(programId);
}

void MyShader::delProgram() {
	glDeleteProgram(programId);
}

void MyShader::setBool(const std::string& name, bool value) const {
	glUniform1i(glGetUniformLocation(programId, name.c_str()), (int)value);
}