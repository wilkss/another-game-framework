#include "agf/renderer/Texture.h"

#include <GL/glew.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace agf {

	Texture::Texture(unsigned width, unsigned height, unsigned target, unsigned internalFormat, unsigned format, unsigned type, unsigned minFilter, unsigned magFilter, unsigned wrap, unsigned depth, const void* pixels)
		: m_Width(width), m_Height(height), m_Target(target), m_Depth(depth)
	{
		glGenTextures(1, &m_Handle);
		glBindTexture(m_Target, m_Handle);

		switch (m_Target)
		{
		case GL_TEXTURE_2D:
			glTexImage2D(m_Target, 0, internalFormat, m_Width, m_Height, 0, format, type, pixels);
			break;
		case GL_TEXTURE_2D_ARRAY:
		case GL_TEXTURE_3D:
			glTexImage3D(m_Target, 0, internalFormat, m_Width, m_Height, depth, 0, format, type, pixels);
			break;
		case GL_TEXTURE_CUBE_MAP:
			for (unsigned i = 0; i < 6; ++i)
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, m_Width, m_Height, 0, format, type, pixels);
			break;
		}

		glTexParameteri(m_Target, GL_TEXTURE_MIN_FILTER, minFilter);
		glTexParameteri(m_Target, GL_TEXTURE_MAG_FILTER, magFilter);
		glTexParameteri(m_Target, GL_TEXTURE_WRAP_S, wrap);
		glTexParameteri(m_Target, GL_TEXTURE_WRAP_T, wrap);
		glGenerateMipmap(m_Target);
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &m_Handle);
	}

	Texture* Texture::LoadTexture(const std::string& path)
	{
		stbi_set_flip_vertically_on_load(false);
		int width;
		int height;
		int components;
		auto* data = stbi_load(path.c_str(), &width, &height, &components, STBI_default);
		if (!data)
			return nullptr;

		GLenum format = GL_RGBA;
		if (components == 1)
			format = GL_RED;
		else if (components == 3)
			format = GL_RGB;
		else if (components == 4)
			format = GL_RGBA;

		Texture* texture = new Texture(width, height, GL_TEXTURE_2D, format, format, GL_UNSIGNED_BYTE, GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE, 1, data);
		stbi_image_free(data);
		return texture;
	}

	void Texture::Bind(unsigned slot)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(m_Target, m_Handle);
	}

	void Texture::Unbind(unsigned slot)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(m_Target, m_Handle);
	}

}