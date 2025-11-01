#pragma once

#include <string>
#include <vector>
using namespace std;

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <assimp/scene.h>

#include "Shader.h"



struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
};

struct Texture {
	GLuint id;
	string type;
	aiString path;
};



class Mesh
{
public:
	// Mesh data
	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<Texture> textures;
	bool hasTangentBitangent = false;

	// Function
	Mesh(const vector<Vertex>& vertices, const vector<GLuint>& indices, const vector<Texture>& textures);
	void Draw(Shader shader);



private:
	// Render data
	GLuint VAO, VBO, EBO;
	// Functions
	void setupMesh();

};




	
