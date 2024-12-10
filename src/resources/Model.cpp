#include "Model.h"

void Model::Draw(Render::ShaderProgram &program)
{
	for (unsigned int i = 0; i < meshes.size(); i++) {
		meshes[i].Draw(program);
	}
}

void Model::loadModel(std::string path)
{
	Assimp::Importer import;
	
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace );
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cerr << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}
	directory = path.substr(0, path.find_last_of('/'));
	
	processNode(scene->mRootNode, scene);
	
}



void Model::processNode(aiNode* node, const aiScene* scene)
{
	//обработать все полигональные сетки в узле(если есть) 
		for (unsigned int i = 0; i < node->mNumMeshes; i++) {
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(processMesh(mesh, scene));
		
		}
	// выполнить то же самое для всех потомков узла рекурсивно
	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(std::unique_ptr<FileMapping>& fMapping, const int& vertSize, const int& indxSize, const int& texSize)
{
	Texture textureMesh;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	
	DWORD nBytes;
	for (int vertIt = 0; vertIt < vertSize; ++vertIt) {
		Vertex vert;
		std::memcpy(&vert,fMapping->dataPtr, sizeof(vert));
		fMapping->dataPtr += sizeof(vert);
	
		vertices.push_back(vert);
	}
	
	for (int indxIt = 0; indxIt < indxSize; ++indxIt) {
		uint32_t indx;
		std::memcpy(&indx, fMapping->dataPtr, sizeof(indx));
		fMapping->dataPtr += sizeof(indx);
	
		indices.push_back(indx);
	}
	MyTextures texes;
	for (int texIt = 0; texIt < texSize; ++texIt) {
		char str[100];								//принято соглашение на статический размер текстовой информации в файле 
	
		std::memcpy(&str, fMapping->dataPtr, 100); //тип текстуры 
		texes.type = str;
		fMapping->dataPtr += 100;
	
		std::memcpy(&str, fMapping->dataPtr, 100);
		texes.fileName = str;						//имя файла текстуры 
		fMapping->dataPtr += 100;
	
		bool skip = false;
		for (unsigned int j = 0; j < loadedTextures.size(); j++) {
			if (std::strcmp(loadedTextures[j].path.data(), texes.fileName.c_str()) == 0) {	//оптимизация с уже загруженными текстурами 

				textures.push_back(loadedTextures[j]);
				skip = true;
				break;
			}
		}
			if (!skip) {
				textureMesh.type = str;
				textureMesh.path = str;
				textureMesh.id = TextureFromFile(textureMesh.path.c_str(), this->directory);
				textures.push_back(textureMesh);
				loadedTextures.push_back(textureMesh);
			}
	}
	return Mesh(vertices, indices, textures);
}

void Model::loadConvertedModel(const char* path)
{
	directory = std::string(path).substr(0,std::string(path).find_last_of('/'));
	HANDLE descrypt = CreateFile(path, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL); //дескриптор файла
	if (descrypt == INVALID_HANDLE_VALUE) {
		std::cerr << "READING FAILED :: " << GetLastError() << std::endl;
		CloseHandle(descrypt);
		return;
	}
	Header head;
	DWORD dwFsize = GetFileSize(descrypt, NULL); 
	if (dwFsize == INVALID_FILE_SIZE) {
		std::cerr << "fileMappingCreate - CreateFile failed, fname = "
			<< descrypt << std::endl;
		CloseHandle(descrypt);
		return;
	}
	HANDLE hMapping = CreateFileMapping(descrypt, nullptr, PAGE_READONLY, 0, 0, nullptr);//создание отображения 
	if (hMapping == nullptr) {
		std::cerr << "fileMappingCreate - CreateFile failed, fname = "
			<< descrypt << std::endl;
		CloseHandle(hMapping);
		CloseHandle(descrypt);
		return;
	}
	unsigned char* dataPtr = (unsigned char*)MapViewOfFile(hMapping, FILE_MAP_READ, 0, 0, dwFsize); //получение адреса отображения в памяти 
	if (dataPtr == nullptr) {
		std::cerr << "fileMappingCreate - CreateFile failed, fname = "
			<< descrypt << std::endl;
		CloseHandle(hMapping);
		CloseHandle(descrypt);
		return;
	}
	std::unique_ptr<FileMapping> mapping(new FileMapping); //создание динамической структуры отображения 
	if (mapping == nullptr) {
		std::cerr << "fileMappingCreate - malloc failed, fname = " << descrypt << std::endl;
		UnmapViewOfFile(dataPtr);
		CloseHandle(hMapping);
		CloseHandle(descrypt);
		return;
	}
	mapping->dataPtr = dataPtr;
	mapping->fsize = (size_t)dwFsize;
	mapping->hFile = descrypt;
	mapping->hMapping = hMapping;

	DWORD nBytes;
	uint32_t numVert;
	uint32_t numIndx;
	uint32_t numTex;
	
	std::memcpy(&head, mapping->dataPtr, sizeof(head)); //считывание заголовка по байтам 
	mapping->dataPtr += sizeof(head);	//смещение адреса через арифметику указателей

	for (int i = 0; i < head.numOfMeshes; ++i) { //считывание кол-ва данных каждого меша 
		std::memcpy(&numVert, mapping->dataPtr, sizeof(uint32_t)); 
		mapping->dataPtr += sizeof(uint32_t);
		std::memcpy(&numIndx, mapping->dataPtr, sizeof(uint32_t));
		mapping->dataPtr += sizeof(uint32_t);
		std::memcpy(&numTex, mapping->dataPtr, sizeof(uint32_t));
		mapping->dataPtr += sizeof(uint32_t);	
		meshes.push_back(processMesh(mapping, numVert, numIndx, numTex)); //считывание и обработка самих данных
	
	}
	UnmapViewOfFile(mapping->dataPtr);
	CloseHandle(mapping->hMapping);
	CloseHandle(mapping->hFile);
}

Mesh Model::processMesh(aiMesh*  mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex; //создание временного объекта структуры vertex
		glm::vec3 vector;
		//перенос координат вершин в буффер
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;
		//здесь открытие файла 
		// 
		//перенос нормалей
		if (mesh->HasNormals()) {
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;
		}
		
		//перенос текстур
		if (mesh->mTextureCoords[0]) {//есть ли текстуры
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
			// tangent
			vector.x = mesh->mTangents[i].x;
			vector.y = mesh->mTangents[i].y;
			vector.z = mesh->mTangents[i].z;
			vertex.Tangent = vector;
			// bitangent
			vector.x = mesh->mBitangents[i].x;
			vector.y = mesh->mBitangents[i].y;
			vector.z = mesh->mBitangents[i].z;
			vertex.Bitangent = vector;
		}
		else vertex.TexCoords = glm::vec2(0.f, 0.f);
		
		vertices.push_back(vertex); 
	}
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}

		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		
		std::vector<Texture> diffuseMaps = loadMaterialTexture(material, 
																	aiTextureType_DIFFUSE, "materials.diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		std::vector<Texture> specularMaps = loadMaterialTexture(material,
																	aiTextureType_SPECULAR, "materials.specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		
		std::vector<Texture> normalMaps = loadMaterialTexture(material, 
																	aiTextureType_NORMALS, "materials.normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		
		std::vector<Texture> heightMaps = loadMaterialTexture(material, 
																	aiTextureType_HEIGHT, "materials.height");
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());


	return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTexture( aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
		unsigned int count = mat->GetTextureCount(type);
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;
		for (unsigned int j = 0; j < loadedTextures.size(); j++) {
			if (std::strcmp(loadedTextures[j].path.data(), str.C_Str()) == 0) {
			
				textures.push_back(loadedTextures[j]);
				skip = true;
				break;
			}
		
		}
		if (!skip) {
			Texture texture;
			texture.id = TextureFromFile(str.C_Str(), this->directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			loadedTextures.push_back(texture);
		}
	}

	return textures;
}


unsigned int Model::TextureFromFile(const char* path, const std::string& directory)
{
	std::string filename = std::string(path);
	filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		switch (nrComponents) {
		case 1: format = GL_RED; break;
		case 3: format = GL_RGB; break;
		case 4: format = GL_RGBA; break;
		default: format = GL_RGB; break;
		}
		
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << filename << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

