#pragma once 
#ifndef COLLIDER_H
#define COLLIDER_H 
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
#include "../render/ShaderProgram.h"
#include <memory>
#include <array>
class GameObject;

class Collider {
public:
    GLuint VAO = 0;
    Collider();
	Collider(const Collider&) = delete;
	Collider& operator=(const Collider&) = delete;
	Collider& operator=(Collider&&) = delete;
	Collider(Collider&&) = delete;
    void drawAroundMesh(Render::ShaderProgram collisionProg);
private:

    GLfloat CollideVert[6*6*3] = {
       -0.5f, -0.5f, -0.5f,  // Bottom-left
       -0.5f,  0.5f, -0.5f,   // top-left
         0.5f,  0.5f, -0.5f,   // top-right
         0.5f, -0.5f, -0.5f,   // bottom-right         
        
        // Front face
        -0.5f, -0.5f,  0.5f,   // bottom-left
         0.5f, -0.5f,  0.5f,  // bottom-right
         0.5f,  0.5f,  0.5f, // top-right
         0.5f,  0.5f,  0.5f,   // top-right
        -0.5f,  0.5f,  0.5f,   // top-left
        -0.5f, -0.5f,  0.5f,  // bottom-left
        // Left face
        -0.5f,  0.5f,  0.5f,   // top-right
        -0.5f,  0.5f, -0.5f,   // top-left
        -0.5f, -0.5f, -0.5f,  // bottom-left
        -0.5f, -0.5f, -0.5f,  // bottom-left
        -0.5f, -0.5f,  0.5f,  // bottom-right
        -0.5f,  0.5f,  0.5f,   // top-right
        // Right face
         0.5f,  0.5f,  0.5f,   // top-left
         0.5f, -0.5f, -0.5f,   // bottom-right
         0.5f,  0.5f, -0.5f,   // top-right         
         0.5f, -0.5f, -0.5f,  // bottom-right
         0.5f,  0.5f,  0.5f,   // top-left
         0.5f, -0.5f,  0.5f,   // bottom-left     
         // Bottom face
         -0.5f, -0.5f, -0.5f,   // top-right
          0.5f, -0.5f, -0.5f,   // top-left
          0.5f, -0.5f,  0.5f,   // bottom-left
          0.5f, -0.5f,  0.5f,  // bottom-left
         -0.5f, -0.5f,  0.5f,   // bottom-right
         -0.5f, -0.5f, -0.5f,   // top-right
         // Top face
         -0.5f,  0.5f, -0.5f,   // top-left
          0.5f,  0.5f,  0.5f,  // bottom-right
          0.5f,  0.5f, -0.5f,   // top-right     
          0.5f,  0.5f,  0.5f, // bottom-right
         -0.5f,  0.5f, -0.5f,   // top-left
         -0.5f,  0.5f,  0.5f  // bottom-left        
    };
    GLuint VBO = 0;
    void setupCubeData();

};
#endif // !COLLIDER_H
