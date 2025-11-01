#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
using namespace std;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <SOIL/SOIL.h>
#include <assimp/types.h>
#include <assimp/Importer.hpp>
#include <assimp/material.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"


GLint TextureFromFile(const char* path, string directory);



class Model
{
public:
	// Model Data
	vector<Texture> textures_loaded;  // Stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once
	vector<Mesh> meshes;
	string directory;
	bool gammaCorrection;


	// Functions
	Model(const char* path, bool gamma = false);
	void Draw(Shader& shader);



private:
	

	// Functions
	void loadModel(string const& path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
};


