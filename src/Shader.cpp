#include "Shader.h"


// read shader files, generate shaders, compile and link them
Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);

		std::stringstream vShaderStream, fShaderStream;
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	
	catch (std::ifstream::failure e)
	{
		std::cout << "EROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	unsigned int vertex, fragment;

	// Vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	checkCompileErrors(vertex, "VERTEX");

	// Fragment shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);

	checkCompileErrors(fragment, "FRAGMENT");

	// Shader program
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	checkCompileErrors(ID, "PROGRAM");


	// delete shaders
	glDeleteShader(vertex);
	glDeleteProgram(fragment);
}

void Shader::use()
{
	glUseProgram(ID);
}


// set bool uniform (input) for our shaders
void Shader::setBool(const std::string& name, bool value)
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}


// set int uniform (input) for our shaders
void Shader::setInt(const std::string& name, int value)
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}


// set float uniform (input) for our shaders
void Shader::setFloat(const std::string& name, float value)
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

// check errors for compiling and linking shaders
void Shader::checkCompileErrors(unsigned int shader, std::string type)
{
	int success;
	char infoLog[512];
	if (type == "PROGRAM")
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);

		if (!success)
		{
			glGetProgramInfoLog(shader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM_LINKING_ERROR of type: "<<type<<"\n" << infoLog << std::endl;
		}
	}
	else
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << std::endl;
		}
	}
}