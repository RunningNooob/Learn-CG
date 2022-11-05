#pragma once
#include <glad/glad.h>
#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>

class MyShader
{
public:
	unsigned int programId;

	std::map<unsigned int, std::string> shaderType_to_shaderNameStr = {
	{GL_VERTEX_SHADER,"VERTEX_SHADER"},
	{GL_FRAGMENT_SHADER,"FRAGMENT_SHADER"}
	};

	MyShader(const std::string& vertaxShaderPath, const std::string& fragmentShaderPath);

	std::string readShader(const std::string& path);

	unsigned int compileShader(unsigned int type, const std::string& source);

	unsigned int createShader(const std::string& vertexShaderSrc, const std::string& fragmentShaderSrc);

	void use();

	void setBool(const std::string& name, bool value) const;

	void setInt(const std::string& name, int value) const;

	void setFloat(const std::string& name, float value) const;

	void setMatrix4fv(const std::string& name, float* value) const;

	void setVec3(const std::string& name, float const * value) const;

	void setMatrix3fv(const std::string& name, float* value) const;


	void delProgram();
};