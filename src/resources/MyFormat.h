#ifndef MYFORMAT_H
#define MYFORMAT_H
#include <stdint.h>
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <string>
#include <vector>
#include <iostream>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>
#include <fstream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../render/ShaderProgram.h"
#include "../render/texture2D.h"
#include <memory>
#include <windows.h>
#include "../render/texture2D.h"
#include <array>

#pragma pack(push, 1)

struct EaxmodHeader {
	uint16_t signature;
	uint16_t numOfMeshes;
};

struct MyVertex {
	// position
	glm::vec3 Position;
	// normal
	glm::vec3 Normal;
	// texCoords
	glm::vec2 TexCoords;
	// tangent
	glm::vec3 Tangent;
	// bitangent
	glm::vec3 Bitangent;

};

struct MyTexture {
	char type[100];
	char fileName[100];
};


#pragma pack(pop)


class MyFormat 
{
public:
	MyFormat () = delete;
	MyFormat(const char* modelPath, const char* whereToSave) {
		convertModel(std::string(modelPath), whereToSave);
	};
	MyFormat(const MyFormat&) = delete;
	MyFormat operator =(const MyFormat&) = delete;
	
private:
	std::vector<MyTexture> loadedTextures;
	std::string nextDirectory;
	std::string lastDirectory;
	void convertModel(std::string sourcePath, const char* path);
	
	
	void processMesh(aiMesh* mesh, const aiScene* scene, const HANDLE &handler);
	void loadMaterialTexture(aiMaterial* mat, aiTextureType type, std::string typeName, const HANDLE& handler);

};		
 
#endif // !MYFORMAT_H