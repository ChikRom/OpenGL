#include "cube.h"
#include <vector>

Cube::Cube(CubeType type)
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	float* vertices = nullptr;
	int countVertices = 0;
	if (type == TEXTURE_CUBE)
	{
		vertices = createTextureCube();
		countVertices = 6 * 4 * (3 + 2);
	}
	else if (type == NORMAL_CUBE)
	{
		vertices = createNormalCube();
		countVertices = 6 * 4 * (3 + 3);
	}
	else if (type == CUBE)
	{
		vertices = createCube();
		countVertices = 6 * 4 * 3;
	}
	else if (type == COLOR_CUBE)
	{
		vertices = createColorCube();
		countVertices = 6 * 4 * (3 + 3);
	}
	else if (type == NORMAL_TEXTURE_CUBE)
	{
		vertices = createNormalTextureCube();
		countVertices = 6 * 4 * (3 + 3 + 2);
	}
	else
		std::cout << "Failed to create cube";

	glBufferData(GL_ARRAY_BUFFER, countVertices * sizeof(vertices), vertices, GL_STATIC_DRAW);
	delete vertices;
	unsigned int indices[] =
	{
		0,2,1,
		3,1,2,
		4,5,6, // riverse
		5,7,6, // riverse
		8, 10,9,
		11,9,10,
		12,14,13,
		15,13,14,
		16,18,17,
		19,17,18,
		20,22,21,
		23,21,22
	};
	numIndices = sizeof(indices) / sizeof(indices[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

float* Cube::createTextureCube()
{
	float* vertices = new float[120]
	{
		// Position					//TextureCoord				
		// Front
		-0.5f, -0.5f,  0.5f,		0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,		0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,		1.0f, 0.0f,
		 0.5f,	0.5f,  0.5f,		1.0f, 1.0f,
		 //Back
		-0.5f, -0.5f, -0.5f,		1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,		1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,		0.0f, 0.0f,
		 0.5f,	0.5f, -0.5f,		0.0f, 1.0f,
		 //Left
		-0.5f, -0.5f, -0.5f,		0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,		0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,		1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,		1.0f, 1.0f,
		//Right
		 0.5f, -0.5f,  0.5f,		0.0f, 0.0f,
		 0.5f,	0.5f,  0.5f,		0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,		1.0f, 0.0f,
		 0.5f,	0.5f, -0.5f,		1.0f, 1.0f,
		 //Down
		-0.5f, -0.5f, -0.5f,		0.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,		0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,		1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,		1.0f, 1.0f,
		 //Up
		-0.5f,  0.5f,  0.5f,		0.0f, 0.0f,
		-0.5f,	0.5f, -0.5f,		0.0f, 1.0f,
		 0.5f,	0.5f,  0.5f,		1.0f, 0.0f,
		 0.5f,	0.5f, -0.5f,		1.0f, 1.0f,
	};
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	return vertices;
}

float* Cube::createCube()
{
	// vertices
	float* vertices = new float[72]
	{
		// Position	
		// Front
		-0.5f, -0.5f,  0.5f,		
		-0.5f,  0.5f,  0.5f,		
		 0.5f, -0.5f,  0.5f,		
		 0.5f,	0.5f,  0.5f,		
		 //Back
		-0.5f, -0.5f, -0.5f,		
		-0.5f,  0.5f, -0.5f,		
		 0.5f, -0.5f, -0.5f,		
		 0.5f,	0.5f, -0.5f,		
		 //Left
		-0.5f, -0.5f, -0.5f,		
		-0.5f,  0.5f, -0.5f,		
		-0.5f, -0.5f,  0.5f,		
		-0.5f,  0.5f,  0.5f,		
		//Right
		 0.5f, -0.5f,  0.5f,		
		 0.5f,	0.5f,  0.5f,		
		 0.5f, -0.5f, -0.5f,		
		 0.5f,	0.5f, -0.5f,		
		 //Down
		-0.5f, -0.5f,  0.5f,		
		-0.5f, -0.5f, -0.5f,		
		 0.5f, -0.5f,  0.5f,		
		 0.5f, -0.5f, -0.5f,		
		 //Up
		-0.5f,  0.5f,  0.5f,		
		 0.5f,	0.5f,  0.5f,		
		-0.5f,  0.5f, -0.5f,		
		 0.5f,	0.5f, -0.5f
	};

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	return vertices;
}


float* Cube::createColorCube()
{
	float* vertices = new float[144]
	{
		// Position					//Color				
		// Front
		-0.5f, -0.5f,  0.5f,		0.0f, 0.0f, 1.0f,	
		-0.5f,  0.5f,  0.5f,		0.0f, 1.0f, 0.0f,	
		 0.5f, -0.5f,  0.5f,		1.0f, 0.0f, 0.0f,	
		 0.5f,	0.5f,  0.5f,		1.0f, 1.0f, 1.0f,	
		 //Back
		-0.5f, -0.5f, -0.5f,		0.0f, 0.0f, 1.0f,	
		-0.5f,  0.5f, -0.5f,		0.0f, 1.0f, 0.0f,	
		 0.5f, -0.5f, -0.5f,		1.0f, 0.0f, 0.0f,	
		 0.5f,	0.5f, -0.5f,		1.0f, 1.0f, 1.0f,	
		 //Left
		-0.5f, -0.5f, -0.5f,		0.0f, 0.0f, 1.0f,	
		-0.5f,  0.5f, -0.5f,		0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,		1.0f, 0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,		1.0f, 1.0f, 1.0f,
		//Right
		 0.5f, -0.5f,  0.5f,		1.0f, 0.0f, 0.0f,
		 0.5f,	0.5f,  0.5f,		1.0f, 1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,		1.0f, 0.0f, 0.0f,
		 0.5f,	0.5f, -0.5f,		1.0f, 1.0f, 1.0f,
		 //Down
		-0.5f, -0.5f,  0.5f,		0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,		0.0f, 0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,		1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,		1.0f, 0.0f, 0.0f,
		 //Up
		-0.5f,  0.5f,  0.5f,		0.0f, 1.0f, 0.0f,
		 0.5f,	0.5f,  0.5f,		1.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,		0.0f, 1.0f, 0.0f,
		 0.5f,	0.5f, -0.5f,		1.0f, 1.0f, 1.0f
	};
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	return vertices;
}

unsigned int Cube::getEBO()
{
	return EBO;
}

unsigned int Cube::getVBO()
{
	return VBO;
}

Cube::~Cube()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

float* Cube::createNormalCube()
{
	float* vertices = new float[144]
	{
		// Position					//Normal Vectors		
		// Front
		-0.5f, -0.5f,  0.5f,		 0.0f, 0.0f,  1.0f,	
		-0.5f,  0.5f,  0.5f,		 0.0f, 0.0f,  1.0f,	
		 0.5f, -0.5f,  0.5f,		 0.0f, 0.0f,  1.0f,	
		 0.5f,	0.5f,  0.5f,		 0.0f, 0.0f,  1.0f,	
		 //Back
		-0.5f, -0.5f, -0.5f,		 0.0f, 0.0f, -1.0f,	
		-0.5f,  0.5f, -0.5f,		 0.0f, 0.0f, -1.0f,	
		 0.5f, -0.5f, -0.5f,		 0.0f, 0.0f, -1.0f,	
		 0.5f,	0.5f, -0.5f,		 0.0f, 0.0f, -1.0f,	
		 //Left
		-0.5f, -0.5f, -0.5f,		-1.0f, 0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,		-1.0f, 0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,		-1.0f, 0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,		-1.0f, 0.0f,  0.0f,
		//Right
		 0.5f, -0.5f,  0.5f,		 1.0f, 0.0f,  0.0f,
		 0.5f,	0.5f,  0.5f,		 1.0f, 0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,		 1.0f, 0.0f,  0.0f,
		 0.5f,	0.5f, -0.5f,		 1.0f, 0.0f,  0.0f,
		 //Down
		-0.5f, -0.5f,  0.5f,		 0.0f, -1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,		 0.0f, -1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,		 0.0f, -1.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,		 0.0f, -1.0f, 0.0f,
		 //Up
		-0.5f,  0.5f,  0.5f,		 0.0f, 1.0f,  0.0f,
		 0.5f,	0.5f,  0.5f,		 0.0f, 1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,		 0.0f, 1.0f,  0.0f,
		 0.5f,	0.5f, -0.5f,		 0.0f, 1.0f,  0.0f,
	};
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	return vertices;
}

float* Cube::createNormalTextureCube()
{
	float* vertices = new float[192]
	{
		// Position					//Normal Vectors		//TextureCoord
		// Front
		-0.5f, -0.5f,  0.5f,		 0.0f, 0.0f,  1.0f,		0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,		 0.0f, 0.0f,  1.0f,		0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,		 0.0f, 0.0f,  1.0f,		1.0f, 0.0f,
		 0.5f,	0.5f,  0.5f,		 0.0f, 0.0f,  1.0f,		1.0f, 1.0f,
		 //Back
		-0.5f, -0.5f, -0.5f,		 0.0f, 0.0f, -1.0f,		1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,		 0.0f, 0.0f, -1.0f,		1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,		 0.0f, 0.0f, -1.0f,		0.0f, 0.0f,
		 0.5f,	0.5f, -0.5f,		 0.0f, 0.0f, -1.0f,		0.0f, 1.0f,
		 //Left
		-0.5f, -0.5f, -0.5f,		-1.0f, 0.0f,  0.0f,		0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,		-1.0f, 0.0f,  0.0f,		0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,		-1.0f, 0.0f,  0.0f,		1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,		-1.0f, 0.0f,  0.0f,		1.0f, 1.0f,
		//Right
		 0.5f, -0.5f,  0.5f,		 1.0f, 0.0f,  0.0f,		0.0f, 0.0f,
		 0.5f,	0.5f,  0.5f,		 1.0f, 0.0f,  0.0f,		0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,		 1.0f, 0.0f,  0.0f,		1.0f, 0.0f,
		 0.5f,	0.5f, -0.5f,		 1.0f, 0.0f,  0.0f,		1.0f, 1.0f,
		 //Down
		-0.5f, -0.5f, -0.5f,		 0.0f, -1.0f, 0.0f,		0.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,		 0.0f, -1.0f, 0.0f,		0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,		 0.0f, -1.0f, 0.0f,		1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,		 0.0f, -1.0f, 0.0f,		1.0f, 1.0f,
		 //Up
		-0.5f,  0.5f,  0.5f,		 0.0f, 1.0f,  0.0f,		0.0f, 0.0f,
		-0.5f,	0.5f, -0.5f,		 0.0f, 1.0f,  0.0f,		0.0f, 1.0f,
		 0.5f,	0.5f,  0.5f,		 0.0f, 1.0f,  0.0f,		1.0f, 0.0f,
		 0.5f,	0.5f, -0.5f,		 0.0f, 1.0f,  0.0f,		1.0f, 1.0f,
	};
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	return vertices;
}

// vertices
//float vertexes[] =
//{
//	// Position					//Color				//TextureCoord
//	// Front
//	-0.5f, -0.5f,  0.5f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f,
//	-0.5f,  0.5f,  0.5f,		0.0f, 1.0f, 0.0f,	0.0f, 1.0f,
//	 0.5f, -0.5f,  0.5f,		1.0f, 0.0f, 0.0f,	1.0f, 0.0f,
//	 0.5f,	0.5f,  0.5f,		1.0f, 1.0f, 1.0f,	1.0f, 1.0f,
//	 //Back
//	-0.5f, -0.5f, -0.5f,		0.0f, 0.0f, 1.0f,	1.0f, 0.0f,
//	-0.5f,  0.5f, -0.5f,		0.0f, 1.0f, 0.0f,	1.0f, 1.0f,
//	 0.5f, -0.5f, -0.5f,		1.0f, 0.0f, 0.0f,	0.0f, 0.0f,
//	 0.5f,	0.5f, -0.5f,		1.0f, 1.0f, 1.0f,	0.0f, 1.0f,
//	 //Left
//	-0.5f, -0.5f, -0.5f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f,
//	-0.5f,  0.5f, -0.5f,		0.0f, 1.0f, 0.0f,	0.0f, 1.0f,
//	-0.5f, -0.5f,  0.5f,		1.0f, 0.0f, 0.0f,	1.0f, 0.0f,
//	-0.5f,  0.5f,  0.5f,		1.0f, 1.0f, 1.0f,	1.0f, 1.0f,
//	//Right
//	 0.5f, -0.5f,  0.5f,		1.0f, 0.0f, 0.0f,	0.0f, 0.0f,
//	 0.5f,	0.5f,  0.5f,		1.0f, 1.0f, 1.0f,	0.0f, 1.0f,
//	 0.5f, -0.5f, -0.5f,		1.0f, 0.0f, 0.0f,	1.0f, 0.0f,
//	 0.5f,	0.5f, -0.5f,		1.0f, 1.0f, 1.0f,	1.0f, 1.0f,
//	 //Down
//	-0.5f, -0.5f,  0.5f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f,
//	-0.5f, -0.5f, -0.5f,		0.0f, 0.0f, 1.0f,	0.0f, 1.0f,
//	 0.5f, -0.5f,  0.5f,		1.0f, 0.0f, 0.0f,	1.0f, 0.0f,
//	 0.5f, -0.5f, -0.5f,		1.0f, 0.0f, 0.0f,	1.0f, 1.0f,
//	 //Up
//	-0.5f,  0.5f,  0.5f,		0.0f, 1.0f, 0.0f,	0.0f, 0.0f,
//	 0.5f,	0.5f,  0.5f,		1.0f, 1.0f, 1.0f,	0.0f, 1.0f,
//	-0.5f,  0.5f, -0.5f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f,
//	 0.5f,	0.5f, -0.5f,		1.0f, 1.0f, 1.0f,	1.0f, 1.0f,

//};

//unsigned int indices[] =
//{
//	0,1,2,
//	3,1,2,
//	4,6,5, // riverse
//	6,7,5, // riverse
//	8, 9,10,
//	11,9,10,
//	12,13,14,
//	15,13,14,
//	16,17,18,
//	19,17,18,
//	20,21,22,
//	23,21,22
//};
//int num_ind = sizeof(indices) / sizeof(indices[0]);

// Set up our VAO, VBO, EBO
/*unsigned int VAO, VBO, EBO;
glGenVertexArrays(1, &VAO);
glGenBuffers(1, &VBO);
glGenBuffers(1, &EBO);

glBindVertexArray(VAO);
glBindBuffer(GL_ARRAY_BUFFER,VBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertexes), vertexes, GL_STATIC_DRAW);
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
glEnableVertexAttribArray(0);
glEnableVertexAttribArray(1);
glEnableVertexAttribArray(2);

glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW)*/;

//glBindBuffer(GL_ARRAY_BUFFER, 0);
//glBindVertexArray(0);

//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);