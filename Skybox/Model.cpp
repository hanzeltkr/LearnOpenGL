#include "Model.h"

vector<Texture> textures_loaded;



Model::Model(const char* path, bool gamma) {
    std::cout << "Model constructor called with path: " << path << std::endl;
    loadModel(string(path));
    std::cout << "Model constructor finished" << std::endl;
}

void Model::Draw(Shader& shader) {
	for (GLuint i = 0; i < this->meshes.size(); i++) {
		this->meshes[i].Draw(shader);
	}
}

void Model::loadModel(string const& path) {
    std::cout << "Loading model: " << path << std::endl;
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(path,
      aiProcess_Triangulate |
      aiProcess_FlipUVs |
      aiProcess_GenNormals |
      aiProcess_JoinIdenticalVertices |
      aiProcess_FlipWindingOrder);

    if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        std::cout << "Tried to load: " << path << std::endl;
        return;
    }
    std::cout << "Model loaded successfully" << std::endl;
    this->directory = path.substr(0, path.find_last_of('/'));
    this->processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene) {
    std::cout << "Processing node: " << node->mName.C_Str() << std::endl;
	// Process all the node's meshes (if any)
	for (GLuint i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		this->meshes.push_back(this->processMesh(mesh, scene));
	}
	
	// Do the same for each children
	for (GLuint i = 0; i < node->mNumChildren; i++) {
		this->processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {  
    vector<Vertex> vertices;  
    vector<GLuint> indices;  
    vector<Texture> textures; 

	// Check if the mesh has tangents and bitangents
    bool hasTangentBitangent = mesh->HasTangentsAndBitangents();


    // Process vertices  
    for (GLuint i = 0; i < mesh->mNumVertices; i++) {  
        Vertex vertex;  

        glm::vec3 vector;  
        // Positions  
        vector.x = mesh->mVertices[i].x;  
        vector.y = mesh->mVertices[i].y;  
        vector.z = mesh->mVertices[i].z;  
        vertex.Position = vector;  

        // Normals  
        if (mesh->HasNormals()) {  
            vector.x = mesh->mNormals[i].x;  
            vector.y = mesh->mNormals[i].y;  
            vector.z = mesh->mNormals[i].z;  
            vertex.Normal = vector;  
        }  

        // Texture Coordinates  
        if (mesh->mTextureCoords[0]) {  
            glm::vec2 vec;
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
            if (hasTangentBitangent) {
                // Fill tangent and bitangent
                glm::vec3 tangentVec;
                tangentVec.x = mesh->mTangents[i].x;
                tangentVec.y = mesh->mTangents[i].y;
                tangentVec.z = mesh->mTangents[i].z;
                vertex.Tangent = tangentVec;

                glm::vec3 bitangentVec;
                bitangentVec.x = mesh->mBitangents[i].x;
                bitangentVec.y = mesh->mBitangents[i].y;
                bitangentVec.z = mesh->mBitangents[i].z;
                vertex.Bitangent = bitangentVec;
            }
            else {
                vertex.Tangent = glm::vec3(0.0f);
                vertex.Bitangent = glm::vec3(0.0f);
            }
        } else {  
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);  
        }  

        vertices.push_back(vertex);  
    }  

    // Process indices  
    for (GLuint i = 0; i < mesh->mNumFaces; i++) {  
        aiFace face = mesh->mFaces[i];  
        for (GLuint j = 0; j < face.mNumIndices; j++) {  
            indices.push_back(face.mIndices[j]);  
        }  
    }  

    // Process materials  
    if (mesh->mMaterialIndex >= 0) {  
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];  
        // 1. diffuse maps  
        vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");  
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());  
        // 2. specular maps  
        vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");  
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());  
        // 3. normal maps  
        std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");  
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());  
        // 4. height maps  
        std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");  
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());  
    }  

    std::cout << "Mesh vertices: " << vertices.size() << ", indices: " << indices.size() << std::endl;

    return Mesh(vertices, indices, textures);  
}

vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName) {
    vector<Texture> textures;
    for (GLuint i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);
		GLboolean skip = false;
        for (GLuint j = 0; j < textures_loaded.size(); j++) {
            if (std::strcmp(textures_loaded[j].path.C_Str(), str.C_Str()) == 0) {
                textures.push_back(textures_loaded[j]);
                skip = true;
				break;
            }
        }
        if (!skip) {
            Texture texture;
            texture.id = TextureFromFile(str.C_Str(), this->directory);
            texture.type = typeName;
            texture.path = str;
            textures.push_back(texture);
			textures_loaded.push_back(texture);
        }
    }
    return textures;
}



GLint TextureFromFile(const char* path, string directory) {
    // Generate texture ID and load texture data
    string filename = string(path);
    filename = directory + '/' + filename;
    std::cout << "Loading texture: " << filename << std::endl;
    GLuint textureID;
    glGenTextures(1, &textureID);

    int width, height;
    unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGB);

    if (!image) {
        std::cout << "Failed to load texture: " << filename << std::endl;
    }

    // Assign texture to ID
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    SOIL_free_image_data(image);

    return textureID;
}