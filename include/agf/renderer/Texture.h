#pragma once

#include <string>

namespace agf {

	class Texture
	{
	public:
		Texture(unsigned width, unsigned height, unsigned target, unsigned internalFormat, unsigned format, unsigned type, unsigned minFilter, unsigned magFilter, unsigned wrap, unsigned depth = 1, const void* pixels = nullptr);
		~Texture();

		static Texture* LoadTexture(const std::string& path);

		void Bind(unsigned slot = 0);
		void Unbind(unsigned slot = 0);

		inline unsigned GetHandle() const { return m_Handle; }
		inline unsigned GetDepth() const { return m_Depth; }

	private:
		int m_Width;
		int m_Height;
		unsigned m_Target;
		unsigned m_Handle;
		unsigned m_Depth;
	};

}