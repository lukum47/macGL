#include "ResourceManager.h"
#include "../render/ShaderProgram.h"
#include "../render/texture2D.h"
#include <sstream>
#include <fstream>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "stb_image.h"

ResourceManager::ResourceManager(const std::string& path)
{
	size_t found = path.find_last_of("/\\");
	m_path = path.substr(0, found);
}

std::shared_ptr<Render::ShaderProgram> ResourceManager::loadShaderProgram(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath)
{
	std::string vertexString = getFileString(vertexPath);
	if (vertexString.empty()) {
		std::cerr << "No vertex shader" << std::endl;
		return nullptr;
	}
	std::string fragmentString = getFileString(fragmentPath);
	if (vertexString.empty()) {
		std::cerr << "No fragment shader" << std::endl;
		return nullptr;
	}

	std::shared_ptr<Render::ShaderProgram> newShader = m_shaderPrograms.emplace(shaderName, std::make_shared<Render::ShaderProgram>(vertexString, fragmentString)).first->second;
	if (newShader->isCompiled()) {
		return newShader;
	}
	std::cerr << "Can't load shader program\n" << "Vertex " << vertexPath << std::endl
		<< "Fragment " << fragmentPath << std::endl;
	return nullptr;
}

std::shared_ptr<Render::ShaderProgram> ResourceManager::getShaderProgram(const std::string& shaderName)
{
	ShaderProgramsMap::const_iterator it = m_shaderPrograms.find(shaderName);
	if (it != m_shaderPrograms.end()) {
		return it->second;
	}
	std::cerr << "Can't find the shader program: " << shaderName << std::endl;
	return nullptr;
}

std::shared_ptr<Render::Texture2D> ResourceManager::loadTexture(const std::string& textureName, const std::string& texturePath)
{
	int channels = 0;
	int width = 0; 
	int height = 0;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* pixels = stbi_load(std::string(m_path + "/" + texturePath).c_str(), &width, &height, &channels, 0);

	if (!pixels) {
		std::cerr << "can't load image " << texturePath << std::endl;
		return nullptr;
	}
	std::shared_ptr<Render::Texture2D> newTexture = m_textures.emplace(textureName, std::make_shared<Render::Texture2D>(width,
																														height, 
																														pixels,
																														channels, 
																														GL_NEAREST,
																														GL_CLAMP_TO_EDGE)).first->second;
	stbi_image_free(pixels);
	return newTexture;

}

std::shared_ptr<Render::Texture2D> ResourceManager::getTextures(const std::string& textureName)
{
	TexturesMap::const_iterator it = m_textures.find(textureName);
	if (it != m_textures.end()) {
		return it->second;
	}
	std::cerr << "Can't find textures: " << textureName << std::endl;
	return nullptr;

}


std::string ResourceManager::getFileString(const std::string& pathToFile) const
{
	std::ifstream f;
	f.open(m_path + "/" + pathToFile.c_str(), std::ios::in | std::ios::binary);
	if (!f.is_open()) {
		std::cerr << "Can't open the file" << pathToFile << std::endl;
		return std::string();
	}
	std::stringstream buffer;
	buffer << f.rdbuf();
	return buffer.str();
	
}
