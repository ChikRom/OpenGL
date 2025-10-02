#ifndef SHADER_H
#define SHADER_H


#include "GLAD/glad/glad.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>



class Shader
{

public:
	unsigned int ID;
public:
	// reads and builds the shader
	Shader(const char* vertexPath, const char* fragmentPath);
	void use(); // activate the shader

	//utility for uniform functions
	void setBool(const std::string& name, bool value);
	void setInt(const std::string& name, int value);
	void setFloat(const std::string& name, float value);

private:
	void checkCompileErrors(unsigned int shader, std::string type);

};



#endif
