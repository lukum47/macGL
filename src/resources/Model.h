#pragma once 
#include "Windows.h"
#include <GLAD/glad.h>
#include <glfw/glfw3.h>
#include <iostream>
#include <string>
#include <vector>
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../render/ShaderProgram.h"
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>
#include "stb_image.h"
#include <array>
//#include "MyFormat.h"

//#define MAX_BONE_INFLUENCE 4
#pragma pack(push, 1)
struct Header {
	uint16_t signature;
	uint16_t numOfMeshes;
};

struct MyTextures {
	std::string type;
	std::string fileName;
};

#pragma pack(pop)
class Model {
public:
	Model(char* path) {
	std::string modelPath = path;
	if (modelPath.substr(modelPath.find_last_of('.'), modelPath.size()) == ".dwu") {
		loadConvertedModel(path);
	}
	else loadModel(modelPath);
	}
	void Draw(Render::ShaderProgram &program);
private:
	
//������ ������
	unsigned int byteCount = 0;
	std::vector<Mesh> meshes;
	std::string directory;
	std::vector<Texture> loadedTextures;
	void loadModel(std::string path);// ����� �������� ������
	 void processNode(aiNode* node, const aiScene* scene);
	 Mesh processMesh(const HANDLE& handler, const int& vertSize, const int& indxSize, const int& texSize);
	 void loadConvertedModel(const char* path);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTexture(aiMaterial* mat, aiTextureType type, std::string typeName);
	unsigned int TextureFromFile(const char* path, const std::string& directory);
};