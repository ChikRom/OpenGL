#ifndef MESH_H
#define MESH_H
#include <GLAD/glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "shader.h"

#define MAX_BONE_INFLUENCE 4
struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	// tangent
	glm::vec3 Tangent;
	// bitangent
	glm::vec3 Bitangent;
	//bone indexes which will influence this vertex
	int m_BoneIDs[MAX_BONE_INFLUENCE];
	//weights from each bone
	float m_Weights[MAX_BONE_INFLUENCE];
};

struct Texture
{
	uint32_t ID;
	std::string type;
	std::string path;
};

class Mesh
{
public:
	uint32_t VAO;
	std::vector<Vertex> mVertices;
	std::vector<uint32_t> mIndices;
	std::vector<Texture> mTextures;
	Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<Texture> textures);
	void Draw(Shader& shader);
private:
	uint32_t VBO, EBO;
	void setupMesh();
};

#endif