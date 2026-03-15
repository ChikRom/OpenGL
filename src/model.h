#ifndef MODEL_H
#define MODEL_H


#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include "mesh.h"
#include "stb_image.h"
uint32_t TextureFromFile(const std::string& path, const std::string& directory, bool gamma = false);

class Model
{
public:
	Model(const std::string& path, bool gamma = false);
	void Draw(Shader& shader);

	std::vector<Mesh> mMeshes;
	std::vector<Texture> mLoadTextures;
	std::string directory;
	bool gammaCorrection;
private:
	void loadModel(const std::string& path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterial(aiMaterial* mat, aiTextureType type, const std::string& typeName);
};

#endif