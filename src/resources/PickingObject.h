#pragma once
#ifndef PICKINGOBJECT_H
#define PICKINGOBJECT_H
#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <string>
#include <vector>
#include <iostream>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>
#include <fstream>
#include "../render/ShaderProgram.h"
#include <memory>
#include <array>

class PickingObject {
public:
	PickingObject() = delete;
	
	~PickingObject();
	void  Init(unsigned int windowWidth, unsigned int windowHeight);
	void EnableWriting();
	void DisableWriting();

	struct PixelInfo {
		unsigned int objID = 0; 
		unsigned int drawID = 0;
		unsigned int primID = 0;
	};
	PixelInfo readPixel(unsigned int x, unsigned int y);

private:
	GLuint FBO = 0; 
	GLuint pickingTexture = 0; 
	GLuint depthTexture = 0;
};



#endif // !PICKINGOBJECT_H
