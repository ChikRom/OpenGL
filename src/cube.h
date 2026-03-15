#ifndef CUBE_H
#define CUBE_H

#include <GLAD/glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
enum CubeType
{
	TEXTURE_CUBE, NORMAL_CUBE, CUBE, COLOR_CUBE, NORMAL_TEXTURE_CUBE
};

class Cube
{
public:
	Cube(CubeType type);
	unsigned int VAO;
	~Cube();
	int numIndices;
private:
	float* createTextureCube();
	float* createNormalCube();
	float* createCube();
	float* createColorCube();
	float* createNormalTextureCube();
	unsigned int getVBO();
	//unsigned int setVBO(unsigned int vbo);
	unsigned int getEBO();
	unsigned int VBO, EBO;

};

#endif