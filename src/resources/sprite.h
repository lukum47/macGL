#pragma once
#ifndef SPRITE_H
#define SPRITE_H
#include <glad/glad.h>
#include <string>
#include <vec2.hpp>
#include <memory>
namespace Render {
	class Sprite {
	public:
		class Texture2D;
		class ShaderProgram;

		Sprite(const std::shared_ptr<Texture2D> Texture, const std::shared_ptr<ShaderProgram> shaderProgram, const glm::vec2& position = glm::vec2(0.f), const glm::vec2& size = glm::vec2(1.f), const float rotation = 0);
		~Sprite();

		Sprite(const Sprite&) = delete;
		Sprite& operator=(const Sprite&) = delete;

		void render() const;
		void setPosition(const glm::vec2& position);
		void setSize(const glm::vec2& size = glm::vec2(1.f));
		void setRotation(const float rotation);
	private:
		std::shared_ptr<Texture2D> m_Texture;
	    std::shared_ptr<ShaderProgram> m_shaderProgram;
	    glm::vec2 m_position;
		glm::vec2 m_size;
		float m_rotation;

	};

}


#endif // !SPRITE_H
