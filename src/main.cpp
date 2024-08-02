#include <GLAD/glad.h>
#include <glfw/glfw3.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>
#include "resources/camera.h"
#include <iostream>
#include "render/ShaderProgram.h"
#include <sstream>
#include <fstream>
#include "resources/ResourceManager.h"
#include "render/texture2D.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void frameBufferSizeCallback(GLFWwindow* window, int width, int height);//Функция для
void processInput(GLFWwindow* window);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);

std::string vertex = "D:/cpp/1/secondProject/res/shader/vertex.txt";
std::string lightVertex = "D:/cpp/1/secondProject/res/shader/vertexLight.txt";

std::string lightFragment = "D:/cpp/1/secondProject/res/shader/fragmentLight.txt";

std::string fragment = "D:/cpp/1/secondProject/res/shader/fragment2.txt";

GLfloat vertecies[] = {
	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,

	-0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,

	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,

	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,

	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f, -0.5f,

	-0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f
};
GLfloat textures[] = {
	0.0f, 0.0f,
	  1.0f, 0.0f,
	   1.0f, 1.0f,
	   1.0f, 1.0f,
	  0.0f, 1.0f,
	  0.0f, 0.0f,

	  0.0f, 0.0f,
	 1.0f, 0.0f,
	   1.0f, 1.0f,
	   1.0f, 1.0f,
	  0.0f, 1.0f,
	  0.0f, 0.0f,

	  1.0f, 0.0f,
	  1.0f, 1.0f,
	 0.0f, 1.0f,
	  0.0f, 1.0f,
	  0.0f, 0.0f,
	  1.0f, 0.0f,

	  1.0f, 0.0f,
	  1.0f, 1.0f,
	  0.0f, 1.0f,
	   0.0f, 1.0f,
	   0.0f, 0.0f,
	   1.0f, 0.0f,

	 0.0f, 1.0f,
	  1.0f, 1.0f,
	   1.0f, 0.0f,
	  1.0f, 0.0f,
	 0.0f, 0.0f,
	  0.0f, 1.0f,

	  0.0f, 1.0f,
	  1.0f, 1.0f,
	  1.0f, 0.0f,
	   1.0f, 0.0f,
	  0.0f, 0.0f,
	 0.0f, 1.0f


};
//GLfloat vertecies[] = {
//		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
//	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
//	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
//	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
//	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
//	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
//
//	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
//	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
//	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
//	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
//	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
//	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
//
//	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
//	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
//	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
//	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
//	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
//	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
//
//	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
//	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
//	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
//	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
//	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
//	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
//
//	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
//	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
//	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
//	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
//	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
//	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
//
//	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
//	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
//	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
//	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
//	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
//	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
//};
//GLfloat colors[] = {
//	1.f, 0.f,0.f,
//	0.f, 1.f, 0.f,
//	0.f, 0.f, 1.f,
//	1.f, 0.f, 0.f,
//	0.f, 0.f, 1.f,
//	1.f, 0.f, 0.f,
//
//	1.f, 0.f,0.f,
//	0.f, 1.f, 0.f,
//	0.f, 0.f, 1.f,
//	1.f, 0.f, 0.f,
//	0.f, 0.f, 1.f,
//	1.f, 0.f, 0.f,
//
//	1.f, 0.f,0.f,
//	0.f, 1.f, 0.f,
//	0.f, 0.f, 1.f,
//	1.f, 0.f, 0.f,
//	0.f, 0.f, 1.f,
//	1.f, 0.f, 0.f,
//
//	1.f, 0.f,0.f,
//	0.f, 1.f, 0.f,
//	0.f, 0.f, 1.f,
//	1.f, 0.f, 0.f,
//
//	1.f, 0.f,0.f,
//	0.f, 1.f, 0.f,
//	0.f, 0.f, 1.f,
//	1.f, 0.f, 0.f,
//	0.f, 0.f, 1.f,
//	1.f, 0.f, 0.f,
//
//	1.f, 0.f,0.f,
//	0.f, 1.f, 0.f,
//	0.f, 0.f, 1.f,
//	1.f, 0.f, 0.f, 
//	0.f, 0.f, 1.f,
//	1.f, 0.f, 0.f
//};
GLfloat normal[] = {
	  0.0f,  0.0f, -1.0f,
	   0.0f,  0.0f, -1.0f,
	 0.0f,  0.0f, -1.0f,
	 0.0f,  0.0f, -1.0f,
	 0.0f,  0.0f, -1.0f,
	  0.0f,  0.0f, -1.0f,

	  0.0f,  0.0f, 1.0f,
	  0.0f,  0.0f, 1.0f,
	   0.0f,  0.0f, 1.0f,
	   0.0f,  0.0f, 1.0f,
	  0.0f,  0.0f, 1.0f,
	  0.0f,  0.0f, 1.0f,

	 -1.0f,  0.0f,  0.0f,
	 -1.0f,  0.0f,  0.0f,
	 -1.0f,  0.0f,  0.0f,
	 -1.0f,  0.0f,  0.0f,
	 -1.0f,  0.0f,  0.0f,
	 -1.0f,  0.0f,  0.0f,

	  1.0f,  0.0f,  0.0f,
	   1.0f,  0.0f,  0.0f,
	   1.0f,  0.0f,  0.0f,
	  1.0f,  0.0f,  0.0f,
	   1.0f,  0.0f,  0.0f,
	  1.0f,  0.0f,  0.0f,

	  0.0f, -1.0f,  0.0f,
	 0.0f, -1.0f,  0.0f,
	 0.0f, -1.0f,  0.0f,
	  0.0f, -1.0f,  0.0f,
	  0.0f, -1.0f,  0.0f,
	  0.0f, -1.0f,  0.0f,

	  0.0f,  1.0f,  0.0f,
	 0.0f,  1.0f,  0.0f,
	   0.0f,  1.0f,  0.0f,
	  0.0f,  1.0f,  0.0f,
	 0.0f,  1.0f,  0.0f,
	  0.0f,  1.0f,  0.0f
};
//GLfloat indices[] = {
//	0,1,2,
//	2,3,1
//};
glm::vec3 cubes[] = {
	glm::vec3(0.f, 0.f, 0.6f),
	glm::vec3(2.f, 0.5f, 0.f),
	glm::vec3(4.f, 1.f, 0.f),
	glm::vec3(6.f, 0.f, 0.7f),
	glm::vec3(8.f, 0.4f, 0.6f)
};

glm::vec3 pointLightPositions[] = {
	glm::vec3(0.7f,  0.2f,  2.0f),
	glm::vec3(2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3(0.0f,  0.0f, -3.0f)
};

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraFixed = glm::vec3(0.0f, 0.0f, -1.0f);




bool firstMouse;
double xPos;
double yPos;
GLfloat lastXPos = 600;
GLfloat lastYPos = 450;
GLfloat yaw = -90.0f;
GLfloat pitch = 0.0f;

glm::vec3 lightPos(0.f, 0.f, 2.f);//глобальная переменная положения источника света
glm::vec3 lampFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 lampUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 lampFixed = glm::vec3(0.0f, 0.0f, -1.0f);



int main(int argc, char** argv) {
	if (!glfwInit()) {
		std::cerr << "FATAL ERROR :: CAN'T INIT GLFW" << std::endl;
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6); 
	int windowWidth = 1200;
	int windowHeight = 900;

	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "OpenGL3D", nullptr, nullptr);//создаем окно и проверяем успешность
	if (window == nullptr) {
		std::cerr << "FATAL ERROR :: CAN'T CREATE WINDOW" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);// делаем созданное окно текущим
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {//инициализируем glad
		std::cerr << "FATAL ERROR :: CAN'T LOAD GLAD" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);
	glClearColor(0.1f, 0.2f, 0.2f, 0.2f);//Задаем цвет фона
	glEnable(GL_DEPTH_TEST);


	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	Render::ShaderProgram cubeProgram(vertex, fragment);
	Render::ShaderProgram lightProgram(lightVertex, lightFragment);
	//cubeProgram.use();
	GLuint pointVBO = 0;// создание буффера вершин
	glGenBuffers(1, &pointVBO);
	glBindBuffer(GL_ARRAY_BUFFER, pointVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertecies), vertecies, GL_STATIC_DRAW);
	
	GLuint textureVBO = 0; //создание буффера цвета
	glGenBuffers(1, &textureVBO);
	glBindBuffer(GL_ARRAY_BUFFER, textureVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(textures), textures, GL_STATIC_DRAW);
	
	GLuint normalVBO = 0;
	glGenBuffers(1, &normalVBO);
	glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normal), normal, GL_STATIC_DRAW);	

	/*GLuint EBO = 0;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/


	GLuint VAO = 0;//создаем массив куда засунем все буфферы
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	
	//glBindBuffer(GL_ARRAY_BUFFER, pointVBO);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);//устанавливаем указатель на вершинный атрибут
	//glEnableVertexAttribArray(0);//привязываем vbo к нулевому эллементу массива

	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	//glEnableVertexAttribArray(1);
	//glBindVertexArray(0);//зануляем
	glBindBuffer(GL_ARRAY_BUFFER, pointVBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(0);
	

	glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(1);
	

	glBindBuffer(GL_ARRAY_BUFFER, textureVBO);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(2);


	int width, height, nrChannels;

	char* path = "D:/cpp/1/openProject/res/textures/container2.png";
	auto image = stbi_load(path, &width, &height, &nrChannels, 0);
	stbi_set_flip_vertically_on_load(true);
	if (!image) {
		std::cerr << "can't load an image" << std::endl;
		glfwTerminate();
		return -1;
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	Render::Texture2D diffuseMap((GLuint)width, (GLuint)height, image, nrChannels, GL_LINEAR_MIPMAP_LINEAR, GL_CLAMP_TO_EDGE);


	path = "D:/cpp/1/openProject/res/textures/container2_specular.png";
	image = stbi_load(path, &width, &height, &nrChannels, 0);
	stbi_set_flip_vertically_on_load(true);
	if (!image) {
		std::cerr << "can't load an image" << std::endl;
		glfwTerminate();
		return -1;
	}
	glBindTexture(GL_TEXTURE_2D, 1);
	Render::Texture2D specularMap((GLint)width, (GLint)height, image, nrChannels, GL_LINEAR_MIPMAP_LINEAR, GL_CLAMP_TO_EDGE);
	
	path = "D:/cpp/1/openProject/res/textures/matrix.jpg";
	image = stbi_load(path, &width, &height, &nrChannels, 0);
	stbi_set_flip_vertically_on_load(true);
	if (!image) {
		std::cerr << "can't load an image" << std::endl;
		glfwTerminate();
		return -1;
	}
	glBindTexture(GL_TEXTURE_2D, 2);
	Render::Texture2D emissionMap((GLint)width, (GLint)height, image, nrChannels, GL_LINEAR_MIPMAP_LINEAR, GL_CLAMP_TO_EDGE);

	path = "D:/cpp/1/openProject/res/textures/container2inverse.png";
	image = stbi_load(path, &width, &height, &nrChannels, 0);
	stbi_set_flip_vertically_on_load(true);
	if (!image) {
		std::cerr << "can't load an image" << std::endl;
		glfwTerminate();
		return -1;
	}
	glBindTexture(GL_TEXTURE_2D, 3);
	Render::Texture2D inverseTexture((GLint)width, (GLint)height, image, nrChannels, GL_LINEAR_MIPMAP_LINEAR, GL_CLAMP_TO_EDGE);

	GLuint lightVAO = 0;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	glBindBuffer(GL_ARRAY_BUFFER, pointVBO);

	/*glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	cubeProgram.use();
	//установка свойств в труктуру materials
	//cubeProgram.setVec3("light.direction", -0.2f, -1.0f, -0.3f);//вектор направления направленного источника света 
	
	cubeProgram.setInt("material.diffuse", 0);
	cubeProgram.setInt("material.specular", 1);
	cubeProgram.setInt("material.emission", 2);
	cubeProgram.setInt("material.inverse", 3);
	cubeProgram.setFloat("material.shininess", 32.f);
	GLfloat emissionValue = 0.f;//устанавливем значение эмиссии 
	Camera camera;

	bool flag = false;
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		processInput(window);
		glfwSetCursorPosCallback(window, mouseCallback);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//активируем программу в цикле
	cubeProgram.use();
	//Свойства объекта
	
	//cubeProgram.setVec3("objectColor", 1.f, 0.5f, 0.31f);//задание цвета освещаемого объекта
	//cubeProgram.setVec3("lightColor", 1.f, 1.f, 1.f);//цвет освещения
	//cubeProgram.setVec3("light.position", lightPos);//позиция источника света
	cubeProgram.setVec3("viewPos", cameraPos);//позиция наблюдателя

	/*glm::vec3 lightColor;
	lightColor.x = sin(glfwGetTime() * 5.0f);
	lightColor.y = sin(glfwGetTime() * 1.8f);
	lightColor.z = sin(glfwGetTime() * 2.6f);*/

	// directional light
	cubeProgram.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
	cubeProgram.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
	cubeProgram.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
	cubeProgram.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
	// point light 1
	cubeProgram.setVec3("pointLights[0].position", pointLightPositions[0]);
	cubeProgram.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
	cubeProgram.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
	cubeProgram.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
	cubeProgram.setFloat("pointLights[0].constant", 1.0f);
	cubeProgram.setFloat("pointLights[0].linear", 0.09f);
	cubeProgram.setFloat("pointLights[0].quadratic", 0.032f);
	// point light 2
	cubeProgram.setVec3("pointLights[1].position", pointLightPositions[1]);
	cubeProgram.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
	cubeProgram.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
	cubeProgram.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
	cubeProgram.setFloat("pointLights[1].constant", 1.0f);
	cubeProgram.setFloat("pointLights[1].linear", 0.09f);
	cubeProgram.setFloat("pointLights[1].quadratic", 0.032f);
	// point light 3
	cubeProgram.setVec3("pointLights[2].position", pointLightPositions[2]);
	cubeProgram.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
	cubeProgram.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
	cubeProgram.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
	cubeProgram.setFloat("pointLights[2].constant", 1.0f);
	cubeProgram.setFloat("pointLights[2].linear", 0.09f);
	cubeProgram.setFloat("pointLights[2].quadratic", 0.032f);
	// point light 4
	cubeProgram.setVec3("pointLights[3].position", pointLightPositions[3]);
	cubeProgram.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
	cubeProgram.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
	cubeProgram.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
	cubeProgram.setFloat("pointLights[3].constant", 1.0f);
	cubeProgram.setFloat("pointLights[3].linear", 0.09f);
	cubeProgram.setFloat("pointLights[3].quadratic", 0.032f);
	// spotLight
	cubeProgram.setVec3("spotLight.position",cameraPos);
	cubeProgram.setVec3("spotLight.direction", cameraFront);
	cubeProgram.setVec3("spotLight.ambient", 0.f, 0.f, 0.f);
	cubeProgram.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
	cubeProgram.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
	cubeProgram.setFloat("spotLight.constant", 1.0f);
	cubeProgram.setFloat("spotLight.linear", 0.09f);
	cubeProgram.setFloat("spotLight.quadratic", 0.032f);
	cubeProgram.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
	cubeProgram.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));


	


	/*glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
	glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);
	glm::vec3 emissionColor = lightColor;*/
	//cubeProgram.setVec3("material.ambient", 0.f, 0.1995f, 0.0745f);
	//cubeProgram.setVec3("material.diffuse", 1.f, 1.f, 1.f);
	//cubeProgram.setVec3("material.specular", 1.f, 1.f, 1.f);
	//cubeProgram.setFloat("material.shinines", 0.4f);
	/*cubeProgram.setVec3("emissionLight", emissionColor);*/
	
	if (flag == false) {
		if (emissionValue >= 3.f) flag = true;
		cubeProgram.setFloat("emissionLight", emissionValue += 0.02f);
		
	}
	if (flag == true) {
		if (emissionValue <= 0.f) flag = false;
		cubeProgram.setFloat("emissionLight", emissionValue -= 0.02f);
		
	}
	
	//установка интенсивности света
	//cubeProgram.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
	//cubeProgram.setVec3("light.diffuse", 1.5f, 1.5f, 1.5f);
	//cubeProgram.setVec3("light.specular", 1.f, 1.f, 1.f);


	//GLint objectColorLoc = cubeProgram.uniformLoc("objectColor");
	//GLint lightColorLoc = cubeProgram.uniformLoc("lightColor");
	//GLint lightPosLoc = cubeProgram.uniformLoc("lightPos");
	//GLint viewPosLoc = cubeProgram.uniformLoc("viewPos");
	//glUniform3f(objectColorLoc, 1.f, 0.5f, 0.31f);
	//glUniform3f(lightColorLoc, 1.f, 1.f, 1.f);
	//glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
	//glUniform3f(viewPosLoc, cameraPos.x, cameraPos.y, cameraPos.z);
	//glm::mat4 model = glm::mat4(1.f);
	//model = glm::translate(model, glm::vec3(0.f, 0.f, 0.f));
	glm::mat4 view = glm::mat4(1.f);

	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	glm::mat4 projection = glm::mat4(1.f);
	projection = glm::perspective(45.0f, (GLfloat)windowWidth / (GLfloat)windowHeight, 0.1f, 100.0f);
	//GLfloat angle = 10.0f; 
//	model = glm::rotate(model, angle, glm::vec3(1.0f, 0.7f, 0.5f));
	GLint modelLoc = cubeProgram.uniformLoc("model");
	GLint viewLoc = cubeProgram.uniformLoc("view");
	GLint projectionLoc = cubeProgram.uniformLoc("projection");
	//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
	
	glBindVertexArray(VAO);
	glm::mat4 model;
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	//model = glm::rotate(model, (GLfloat)glfwGetTime() * glm::radians(90.f), glm::vec3(0.f, 1.f, 0.f));
	
	
	glActiveTexture(GL_TEXTURE0);
	diffuseMap.bind();

	glActiveTexture(GL_TEXTURE1);
	specularMap.bind();
	glActiveTexture(GL_TEXTURE2);
	emissionMap.bind();
	glActiveTexture(GL_TEXTURE3);
	inverseTexture.bind();
	
	for (int i = 0; i < 5; ++i) {
		glm::mat4 model = glm::mat4(1.f);
		model = glm::translate(model, cubes[i]);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	
	glBindVertexArray(0);

	lightProgram.use();
	
	modelLoc = lightProgram.uniformLoc("model");
	viewLoc = lightProgram.uniformLoc("view");
	projectionLoc = lightProgram.uniformLoc("projection");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
	for (int i = 0; i < 4; ++i) {
		model = glm::mat4(1.f);
		model = glm::translate(model, pointLightPositions[i]);
		model = glm::scale(model, glm::vec3(0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	
	
	glBindVertexArray(0);

	glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &pointVBO);
	/*glDeleteBuffers(1, &colorVBO);*/
	glfwTerminate();
	return 0;
}

void frameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	GLfloat cameraSpeed = 0.1f;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cameraPos += cameraSpeed * cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cameraPos -= cameraSpeed * cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		lightPos += cameraSpeed * lampFront;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		lightPos -= cameraSpeed * lampFront;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		lightPos -= glm::normalize(glm::cross(lampFront, lampUp)) * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		lightPos += glm::normalize(glm::cross(lampFront, lampUp)) * cameraSpeed;
	}
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastXPos = xpos;
		lastYPos = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastXPos;
	GLfloat yoffset = lastYPos - ypos;
	lastXPos = xpos;
	lastYPos = ypos;

	GLfloat sensitivity = 0.05;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
	lampFront = front;
	front.y = 0;
	cameraFixed = glm::normalize(front);
	
}

