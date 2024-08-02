#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <vector>

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>

//Определяем несколько возможных опций для движения камеры. Абстракция для специфичных оконных методов ввода
// Используем enum для перечисления
enum cameraMovement {
	FORWARD,
	BACKWARD, 
	LEFT,
	RIGHT
};

//Значения камеры по уммолчанию
const GLfloat YAW = -90.f;
const GLfloat PITCH = 0.f;
const GLfloat SPEED = 3.f;
const GLfloat SENSITIVITY = 0.25f;
const GLfloat ZOOM = 45.f;


//Класс для инкапсуляции методов ввода, матриц и рассчетов
class Camera {
public:
	glm::vec3 Position;//Координата позиции камеры
	glm::vec3 Front;//Наблюдаемый объект
	glm::vec3 Up;//Вектор по оси y
	glm::vec3 Right;//Вектор по оси x
	glm::vec3 WorldUp;//
	//Углы Эйлера
	GLfloat Yaw;
	GLfloat Pitch;
	//Опции камеры
	GLfloat movementSpeed;
	GLfloat mouseSensitivity;
	GLfloat zoom;
	//Конструктор векторов
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH);
	//Конструктор скалярных значений
	Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch);
	//Рассчет матрицы вида 
	glm::mat4 getViewMatrix();
	// функция для отслеживания нажатия клавиш
	void processKeyboard(cameraMovement direction, GLfloat deltaTime);
	void processMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true);
	void processMouseScroll(GLfloat yoffset);
private:
	void updateCameraVectors();
	
};
#endif // !CAMERA_H
