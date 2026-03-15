#include "model.h"

Model::Model(const std::string& path, bool gamma)
	: gammaCorrection(gamma)
{
	loadModel(path);
}

void Model::Draw(Shader& shader)
{
	for (uint32_t i = 0; i < mMeshes.size(); i++)
	{
		mMeshes[i].Draw(shader);
	}
}

void Model::loadModel(const std::string& path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		return;
	}

	directory = path.substr(0, path.find_last_of('/'));

	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	for (uint32_t i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		mMeshes.push_back(processMesh(mesh, scene));
	}

	for (uint32_t i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	std::vector<Texture> textures;

	for (uint32_t i = 0; i < mesh->mNumVertices; i++)
	{
		// Vertices
		Vertex vertex;
		// positions
		glm::vec3 tempVector;
		tempVector.x = mesh->mVertices[i].x;
		tempVector.y = mesh->mVertices[i].y;
		tempVector.z = mesh->mVertices[i].z;
		vertex.Position = tempVector;
		// normals
		if (mesh->HasNormals())
		{
			tempVector.x = mesh->mNormals[i].x;
			tempVector.y = mesh->mNormals[i].y;
			tempVector.z = mesh->mNormals[i].z;
			vertex.Normal = tempVector;
		}
		// texture coordinates
		if (mesh->mTextureCoords[0])
		{
			tempVector.x = mesh->mTextureCoords[0][i].x;
			tempVector.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = glm::vec2(tempVector.x, tempVector.y);
			// tangent
			tempVector.x = mesh->mTangents[i].x;
			tempVector.y = mesh->mTangents[i].y;
			tempVector.z = mesh->mTangents[i].z;
			vertex.Tangent = tempVector;
			// bitangent
			tempVector.x = mesh->mBitangents[i].x;
			tempVector.y = mesh->mBitangents[i].y;
			tempVector.z = mesh->mBitangents[i].z;
			vertex.Bitangent = tempVector;
		}
		else
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);

		vertices.push_back(vertex);
	}

	// Indices
	for (uint32_t i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (uint32_t j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}
	// textures/materials
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	// 1. diffuse maps
	std::vector<Texture> diffuseMaps = loadMaterial(material, aiTextureType_DIFFUSE, "texture_diffuse");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	// 2. specular maps
	std::vector<Texture> specularMaps = loadMaterial(material, aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	// 3. normal maps
	std::vector<Texture> normalMaps = loadMaterial(material, aiTextureType_HEIGHT, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    // 4. height maps
    std::vector<Texture> heightMaps = loadMaterial(material, aiTextureType_AMBIENT, "texture_height");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

	return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterial(aiMaterial* mat, aiTextureType type, const std::string& typeName)
{
	std::vector<Texture> textures;
	for (uint32_t i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;
		for (uint32_t j = 0; j < mLoadTextures.size(); j++)
		{
			if (std::strcmp(mLoadTextures[j].path.data(), str.C_Str()) == 0)
			{
				textures.push_back(mLoadTextures[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{
			Texture texture;
			texture.ID = TextureFromFile(str.C_Str(), directory, gammaCorrection);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			mLoadTextures.push_back(texture);
		}
	}
	return textures;
}

uint32_t TextureFromFile(const std::string& path, const std::string& directory, bool gamma)
{
	std::string filename = directory + "/" + path;

	uint32_t textureID;
	glGenTextures(1, &textureID);
	
	int width, height, nrComponents;

	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format, srgbFormat;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		
		if (!gamma)
			srgbFormat = (nrComponents == 4) ? (GL_SRGB_ALPHA) : (GL_SRGB);
		else
			srgbFormat = format;
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, srgbFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
	}
	stbi_image_free(data);
	return textureID;
}