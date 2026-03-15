#ifndef SHADER_H
#define SHADER_H


#include <GLAD/glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "resource_macro.h"
class Shader
{
public:
	unsigned int ID;
public:
	Shader(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath = "");
	void use() const;
	~Shader();

	// set the uniforms
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setVec2(const std::string& name, const glm::vec2& vector) const;
	void setVec3(const std::string& name, const glm::vec3& vector) const;
	void setVec3(const std::string& name, float x, float y, float z) const;
	void setVec4(const std::string& name, const glm::vec4& vector) const;
	void setVec4(const std::string& name, float x, float y, float z, float w) const;
	void setMat4(const std::string& name, const::glm::mat4& matrix) const;
private:
	void checkCompileErrors(unsigned int shader, const std::string& type);

};


#endif