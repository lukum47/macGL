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
		bool isCompiled() const { //������� ��� �������� ���������� ��������
			return m_isCompiled;
		}
		void setMtrix4(const std::string& name, const glm::mat4& matrix);
		void setInt(const std::string& name, const GLint value);
		ShaderProgram() = delete;//������� ����������� ��� ���������� ��� ����� �������� ���
		ShaderProgram(ShaderProgram&) = delete;//��������� �������� ��� ��������� ���������,
									//�� ���������� ���������� 2 ���� ������� ���� � �� �� ������
		ShaderProgram& operator= (const ShaderProgram&) = delete; //��������� �� �� �����, ��� � ����
													//�� ����������� 2 id ��������� ��������� �� ������� 2 ��������� � ����������� id
		
		ShaderProgram& operator=(ShaderProgram&& shaderProgram) noexcept;//������������� � move
		ShaderProgram(ShaderProgram&& shaderProgram) noexcept;//����������� � move ��������� ������������� ������
		void use() const;//������� ��� ��������� ��������
		GLuint getProgram() {
			return m_ID;
		}
		GLint uniformLoc(const char* name);
		void setVec3(const char* name,  GLfloat x,  GLfloat y,  GLfloat z);
		void setVec3(const char* name, glm::vec3 &vec);
		void setFloat(const char* name, GLfloat value);
	private:
		std::string getFileString(const std::string& pathToFile);
		bool createShader(const std::string& source, const GLenum shaderType, GLuint &shaderID);//������� ��� ���������� �������
		bool m_isCompiled = false; // ���� �������� ��� �� �������� ���������� ��������
		GLuint m_ID = 0; // id ��������� ���������

	};
}


#endif // !SHADERPROGRAM_H
