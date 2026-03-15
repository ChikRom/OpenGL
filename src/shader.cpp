#include "shader.h"
#include "resource_macro.h"

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath)
{
	std::string vShaderCode, fShaderCode, gShaderCode;
	std::fstream vShaderFile, fShaderFile, gShaderFile;



	try
	{
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);

		std::stringstream vStream, fStream;
		vStream << vShaderFile.rdbuf();
		fStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		vShaderCode = vStream.str();
		fShaderCode = fStream.str();

		if (geometryPath != "")
		{
			gShaderFile.open(geometryPath);
			std::stringstream gStream;
			gStream << gShaderFile.rdbuf();
			gShaderFile.close();
			gShaderCode = gStream.str();
		}

	}

	catch (std::fstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
	}

	unsigned int vShader, fShader, gShader;

	const char* vertexCode = vShaderCode.c_str();
	const char* fragmentCode = fShaderCode.c_str();

	// create vertex shader
	vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vertexCode, NULL);
	glCompileShader(vShader);
	checkCompileErrors(vShader, "VERTEX");

	// create fragment shader
	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fragmentCode, NULL);
	glCompileShader(fShader);
	checkCompileErrors(fShader, "FRAGMENT");

	this->ID = glCreateProgram();
	glAttachShader(this->ID, vShader);
	glAttachShader(this->ID, fShader);
	
	// create and attach geometry shader
	if (geometryPath != "")
	{
		const char* geometryCode = gShaderCode.c_str();
		gShader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(gShader, 1, &geometryCode, NULL);
		glCompileShader(gShader);
		checkCompileErrors(gShader, "GEOMETRY");
		glAttachShader(this->ID, gShader);
		glDeleteShader(gShader);
	}
	glLinkProgram(this->ID);
	checkCompileErrors(this->ID, "PROGRAM");

	// delete shaders
	glDeleteShader(vShader);
	glDeleteShader(fShader);

	
}

Shader::~Shader()
{
	glDeleteProgram(this->ID);
}
void Shader::checkCompileErrors(unsigned int shader, const std::string& type)
{
	int success;
	char infoLog[512];
	if (type == "PROGRAM")
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << std::endl;
		}
	}
	else
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM_COMPILATION_ERROR of type: " << type << "\n" << infoLog << std::endl;
		}
			
	}
}

void Shader::use() const
{
	glUseProgram(this->ID);
}

void Shader::setBool(const std::string& name, bool value) const
{
	this->use();
	glUniform1i(glGetUniformLocation(this->ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const
{
	this->use();
	glUniform1i(glGetUniformLocation(this->ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
	this->use();
	glUniform1f(glGetUniformLocation(this->ID, name.c_str()), value);
}

void Shader::setVec2(const std::string& name, const glm::vec2& vector) const
{
	this->use();
	glUniform2fv(glGetUniformLocation(this->ID, name.c_str()), 1, &vector[0]);
}

void Shader::setVec3(const std::string& name, const glm::vec3& vector) const
{
	this->use();
	glUniform3fv(glGetUniformLocation(this->ID, name.c_str()), 1, &vector[0]);
}

void Shader::setVec3(const std::string& name, float x, float y, float z) const
{
	this->use();
	glUniform3f(glGetUniformLocation(this->ID, name.c_str()), x, y, z);
}

void Shader::setVec4(const std::string& name, const glm::vec4& vector) const
{
	this->use();
	glUniform4fv(glGetUniformLocation(this->ID, name.c_str()), 1, &vector[0]);
}

void Shader::setVec4(const std::string& name, float x, float y, float z, float w) const
{
	this->use();
	glUniform4f(glGetUniformLocation(this->ID, name.c_str()), x, y, z, w);
}

void Shader::setMat4(const std::string& name, const::glm::mat4& matrix) const
{
	this->use();
	glUniformMatrix4fv(glGetUniformLocation(this->ID, name.c_str()), 1, GL_FALSE, &matrix[0][0]);
}
