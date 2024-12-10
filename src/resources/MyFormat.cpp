#include "MyFormat.h"

void MyFormat::convertModel(std::string sourcePath, const char* path)
{
	lastDirectory = sourcePath.substr(sourcePath.find_last_of('.'), sourcePath.size());
    if (lastDirectory == ".dwu") {
        std::cout << "FILE IS ALREADY IN CORRECT FORMAT" << std::endl;
        return;
    }
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(sourcePath.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }
	// пути для чтения и записи
    lastDirectory = sourcePath.substr(0, sourcePath.find_last_of('/'));
	nextDirectory = std::string(path).substr(0, std::string(path).find_last_of('/'));
    EaxmodHeader header;
    header.numOfMeshes = scene->mNumMeshes;
	//дескриптор файла по пути для записи
	HANDLE handler = CreateFile(path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (handler == INVALID_HANDLE_VALUE) {
		std::cerr << "CREATION FILE FAILED :: " << GetLastError() << std::endl;
		return;
	}
	DWORD  dwCount;
	WriteFile(handler, &header, sizeof(header), &dwCount, NULL);//считываем заголовок файла
    for (int i = 0; i < scene->mNumMeshes; ++i) {
        aiMesh* mesh = scene->mMeshes[i];

        processMesh(mesh, scene, handler); //счмтываем данные мешей и сохраняем в файл
    }

	CloseHandle(handler);
}


void MyFormat::processMesh(aiMesh* mesh, const aiScene* scene, const HANDLE& handler)
{
	DWORD  dwCount;
	uint32_t countBuf = mesh->mNumVertices;//счетчик кол-ва данных для дальнейшего считывания 
	WriteFile(handler, &countBuf, sizeof(uint32_t), &dwCount, NULL);//сохраняем кол-во вертексов 
	countBuf = mesh->mNumFaces * 3;
	WriteFile(handler, &countBuf, sizeof(uint32_t), &dwCount, NULL);//созраняем кол-во индексов 
	countBuf = scene->mMaterials[mesh->mMaterialIndex]->GetTextureCount(aiTextureType_DIFFUSE); //считываем кол-во текстур по тегам 
	countBuf += scene->mMaterials[mesh->mMaterialIndex]->GetTextureCount(aiTextureType_SPECULAR);
	countBuf += scene->mMaterials[mesh->mMaterialIndex]->GetTextureCount(aiTextureType_NORMALS);
	countBuf += scene->mMaterials[mesh->mMaterialIndex]->GetTextureCount(aiTextureType_HEIGHT);
	WriteFile(handler, &countBuf, sizeof(uint32_t), &dwCount, NULL);
	
	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		MyVertex vertex; //создание временного объекта структуры vertex
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
	
		WriteFile(handler, &vertex, sizeof(vertex), &dwCount, NULL); //запись структуры vertex
			if (dwCount != sizeof(vertex)) {
				std::cerr << "INVALID SAVING VERTEX DATA MESH NUM: " << i << std::endl;
			}
	}
	
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			WriteFile(handler, &face.mIndices[j], sizeof(uint32_t), &dwCount, NULL);//запись индекса
			if (dwCount != sizeof(uint32_t)) {
				std::cerr << "INVALID SAVING INDEX DATA MESH NUM: " << i << std::endl;
			}
			
		}
	}
	

	 aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

	 loadMaterialTexture(material,aiTextureType_DIFFUSE, "materials.diffuse", handler);
	 loadMaterialTexture(material,aiTextureType_SPECULAR, "materials.specular", handler);
	 loadMaterialTexture(material,aiTextureType_NORMALS, "materials.normal", handler);
	 loadMaterialTexture(material,aiTextureType_HEIGHT, "materials.height", handler);
	
}

void MyFormat::loadMaterialTexture(aiMaterial* mat, aiTextureType type, std::string typeName, const HANDLE& handler)
{
	DWORD  dwCount;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
		unsigned int count = mat->GetTextureCount(type);
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;
		for (unsigned int j = 0; j < loadedTextures.size(); j++) {// если в массиве есть ранее считанные структуры, 
			if (std::strcmp(loadedTextures[j].fileName, str.C_Str()) == 0) {// то пропускаем создание и записываем в файл уже существующую текстуру

				WriteFile(handler, &loadedTextures[j], sizeof(MyTexture), &dwCount, NULL);
				skip = true;
				break;
			}

		}
		if (!skip) {
			MyTexture texture;
			strcpy(texture.type, typeName.c_str());
			strcpy(texture.fileName, str.C_Str());
			WriteFile(handler, &texture, sizeof(MyTexture), &dwCount, NULL);
			std::string last = this->lastDirectory + '/' + texture.fileName;
			std::string next = this->nextDirectory + '/' + texture.fileName;
			CopyFile(last.c_str(), next.c_str(), NULL);
			loadedTextures.push_back(texture);
		}
	}
}




