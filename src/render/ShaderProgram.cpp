#include "ShaderProgram.h"
#include <iostream>

namespace Render {
	ShaderProgram::ShaderProgram(const char* vertexShader, const char* fragmentShader)
	{
		GLuint vertexShaderID = 0; // объ€вление вертексного шейдера 
		if (!createShader(vertexShader, GL_VERTEX_SHADER, vertexShaderID)) {//проверка компил€ции вертексного шейдера
			std::cerr << "VERTEX SHADER: Compile-time error:\n";
			return;
		}

		GLuint fragmentShaderID = 0; // объ€вление фрагментного шейдера 
		if (!createShader(fragmentShader, GL_FRAGMENT_SHADER, fragmentShaderID)) {//проверка компил€ции фрагментного шейдера
			std::cerr << "FRAGMENT SHADER: Compile-time error:\n";
			glDeleteShader(vertexShaderID);//при неудачной компил€ции фрагментного шейдера удал€етс€ вертексный
			return;
		}

		m_ID = glCreateProgram();//создание шейдерной программы
		glAttachShader(m_ID, vertexShaderID);//присоедин€ем шейдеры к программе
		glAttachShader(m_ID, fragmentShaderID);
		glLinkProgram(m_ID);//линкуем
		//проверка успешности линковки
		GLint success;
		glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
		if (!success) {
			GLchar inflog[1024];			
			glGetShaderInfoLog(m_ID, 1024, nullptr, inflog); // функци€ вывода сообщени€
			std::cerr << "ERROR::SHADER: Link-time error:\n" << inflog << std::endl;// вывод сообщени€ об ошибке
			return;
		}
		else {
			m_isCompiled = true;//устанавлеваем флаг удачной компил€ции на true
		}
		//после удачной компил€ции программы удал€ем шейдеры, 
		//тк после создани€ шейдерной программы они больше не требуютс€
		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);
	}

	ShaderProgram::ShaderProgram(const std::string pathToVertexShader, const std::string pathToFragmentShader)
	{
		
		GLuint vertexShaderId = 0; 
		if (!createShader(this->getFileString(pathToVertexShader), GL_VERTEX_SHADER, vertexShaderId)) {
			std::cerr << "VERTEX SHADER: Compile-time error:\n";
			return;
		}
		GLuint fragmentShaderId = 0;
		if (!createShader(this->getFileString(pathToFragmentShader), GL_FRAGMENT_SHADER, fragmentShaderId)) {
			std::cerr << "VERTEX SHADER: Compile-time error:\n";
			return;
		}

		m_ID = glCreateProgram();
		glAttachShader(m_ID, vertexShaderId);
		glAttachShader(m_ID, fragmentShaderId);
		glLinkProgram(m_ID);
		GLint success;

		glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
		if (!success) {
			GLchar infolog[512];
			glGetShaderInfoLog(m_ID, 512, nullptr, infolog);
			std::cerr << "ERROR::SHADER: Link-time error:\n" << infolog << std::endl;
			return;
		}
		else {
			m_isCompiled = true;
		}
		glDeleteShader(vertexShaderId);
		glDeleteShader(fragmentShaderId);
	}

	
	GLint ShaderProgram::uniformLoc(const char* name)
	{
		return glGetUniformLocation(m_ID, name);
	}

	std::string ShaderProgram::getFileString(const std::string& pathToFile)
	{
		std::ifstream f;
		f.open(pathToFile.c_str());
		if (!f.is_open()) {
			std::cerr << "Can't open the file" << pathToFile << std::endl;
			return std::string();
		}
		std::stringstream buffer;
		buffer << f.rdbuf();
		return buffer.str();
	}

	void ShaderProgram::setVec3(const char* name, GLfloat x,  GLfloat y,  GLfloat z)
	{
		GLint objectLoc = glGetUniformLocation(m_ID, name);
		glUniform3f(objectLoc, x, y, z);
	}

	bool ShaderProgram::createShader(const std::string& source, const GLenum shaderType, GLuint &shaderID)
	{
		shaderID = glCreateShader(shaderType); // идентификатор шейдера (вертексный или фрагментный)
		const char* code = source.c_str();
		glShaderSource(shaderID, 1, &code, nullptr);// функци€ загрузки шейдера
													//1) идентификатор шейдера
		glCompileShader(shaderID); //компил€ци€ созданного шейдера
		
		GLint success;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);//функци€ проверки  шейдера
															//задаетс€ тип шейдера, вторым параметром, что провер€етс€
															//в данном случае компил€ци€ 
															// третьим параметром куда записать результат
		if (!success) {
			GLchar inflog[1024];// создание буфера дл€ сообщени€ об ошибке указыва€ размер, 
								// как правило одного кб хватает
			glGetShaderInfoLog(shaderID, 1024, nullptr, inflog); // функци€ вывода сообщени€
			std::cerr << "ERROR::SHADER: Compile-time error:\n" << inflog << std::endl;// вывод сообщени€ об ошибке
			return false;
		}
		return true;
	}

	void ShaderProgram::setVec3(const char* name, glm::vec3& vec)
	{
		GLint objectLoc = glGetUniformLocation(m_ID, name);
		glUniform3f(objectLoc, vec.x, vec.y, vec.z);
	}

	void ShaderProgram::setFloat(const char* name, GLfloat value)
	{
		GLint floatLoc = glGetUniformLocation(m_ID, name);
		glUniform1f(floatLoc, value);

	}

	ShaderProgram::~ShaderProgram()
	{
		glDeleteProgram(m_ID);//в деструкторе удал€ем шейдерную программу
	}

	void ShaderProgram::setMtrix4(const std::string& name, const glm::mat4& matrix)
	{
		glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void ShaderProgram::setInt(const std::string& name, const GLint value)
	{
		glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
	}

	ShaderProgram& ShaderProgram::operator=(ShaderProgram&& shaderProgram) noexcept
	{
		glDeleteProgram(m_ID); //удал€ем старую программу
		m_ID = shaderProgram.m_ID; // в пустое место заносим программу
		m_isCompiled = shaderProgram.m_isCompiled; //приравниваем значение флага
	
		shaderProgram.m_ID = 0; //старые значени€ занул€ем чтобы деструктор
		shaderProgram.m_isCompiled = false;//не пыталс€ повторно удалить данные
		return *this;
	}

	ShaderProgram::ShaderProgram(ShaderProgram&& shaderProgram) noexcept
	{
		m_ID = shaderProgram.m_ID; // в пустое место заносим программу
		m_isCompiled = shaderProgram.m_isCompiled; //приравниваем занчение флага

		shaderProgram.m_ID = 0; //старые значени€ занул€ем чтобы деструктор
		shaderProgram.m_isCompiled = false;//не пыталс€ повторно удалить данные
	}

	void ShaderProgram::use() const
	{
		glUseProgram(m_ID); //функци€ использовани€ созданной шейдерной программы
	}

}