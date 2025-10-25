#ifndef MODEL_H
#define MODEL_H

#include "Shader.h"
#include "Mesh.h"
#include "assimp/Importer.hpp"
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "stb_image.h"
class Model
{
public:
	vector<Texture> textures_loaded;
	vector<Mesh> meshes;
	string directory;


	Model(const string& path);
	void Draw(Shader& shader);
private:

	void loadModel(const string& path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
};

#endif MODEL_H
