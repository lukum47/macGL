#pragma once
#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <glad/glad.h>
#include <string>
#include <mat4x4.hpp>
#include <gtc/type_ptr.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
namespace Render {
	class ShaderProgram {
	public:
		ShaderProgram(const char* vertexShader, const char* fragmentShader);
		ShaderProgram(const std::string pathToVertexShader, const std::string pathToFragmentShader);
		~ShaderProgram();
		bool isCompiled() const { //функци€ дл€ проверки компил€ции шейдеров
			return m_isCompiled;
		}
		void setMtrix4(const std::string& name, const glm::mat4& matrix);
		void setInt(const std::string& name, const GLint value);
		ShaderProgram() = delete;//удал€ем конструктор без параметров тем самым запреща€ его
		ShaderProgram(ShaderProgram&) = delete;//исключаем создание уже созданной программы,
									//тк деструктор попытаетс€ 2 раза удалить одни и те же данные
		ShaderProgram& operator= (const ShaderProgram&) = delete; //исключаем то же самое, что и выше
													//тк приравнива€ 2 id шейдерной программы мы создаем 2 программы с одинаковыми id
		
		ShaderProgram& operator=(ShaderProgram&& shaderProgram) noexcept;//приравнивание с move
		ShaderProgram(ShaderProgram&& shaderProgram) noexcept;//конструктор с move использу€ универсальные ссылки
		void use() const;//функци€ дл€ включени€ шейдеров
		GLuint getProgram() {
			return m_ID;
		}
		GLint uniformLoc(const char* name);
		void setVec3(const char* name,  GLfloat x,  GLfloat y,  GLfloat z);
		void setVec3(const char* name, glm::vec3 &vec);
		void setFloat(const char* name, GLfloat value);
	private:
		std::string getFileString(const std::string& pathToFile);
		bool createShader(const std::string& source, const GLenum shaderType, GLuint &shaderID);//функци€ дл€ реализации шейдера
		bool m_isCompiled = false; // флаг успешна€ или не успешна€ компил€ци€ шейдеров
		GLuint m_ID = 0; // id шейдерной программы

	};
}


#endif // !SHADERPROGRAM_H
