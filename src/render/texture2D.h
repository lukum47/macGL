#pragma once
#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include <glad/glad.h>
#include <string>

namespace Render {
	class Texture2D
	{
	public:
		Texture2D(const GLuint width, GLuint height, const unsigned char* data, const unsigned int chanels = 4, const GLenum filter = GL_NEAREST, const GLenum wrapMode = GL_CLAMP_TO_EDGE);
	
		~Texture2D();

		Texture2D() = delete;
		Texture2D(const Texture2D&) = delete;
		Texture2D& operator=(const Texture2D&) = delete;
		Texture2D& operator=(Texture2D&& texture2d);
		Texture2D(Texture2D&& texture2d);

		void bind() const;

	private:
		GLuint m_ID;
		GLenum m_mode;
		unsigned int m_width = 0;
		unsigned int m_height = 0;
	};
}


#endif // !TEXTURE2D_H
