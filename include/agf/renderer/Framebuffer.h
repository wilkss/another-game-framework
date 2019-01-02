#pragma once

#include <vector>

namespace agf {
	
	class Texture;

	class Framebuffer
	{
	public:
		Framebuffer(unsigned width, unsigned height, const std::vector<Texture*>& colorTextures, Texture* depthTexture = nullptr, unsigned layer = 0);
		~Framebuffer();

		void Bind() const;
		void Unbind() const;

		inline unsigned GetWidth() const { return m_Width; }
		inline unsigned GetHeight() const { return m_Height; }

	private:
		unsigned m_Width;
		unsigned m_Height;
		unsigned m_Handle;
		std::vector<Texture*> m_ColorTextures;
		unsigned m_DepthRenderbufferHandle;
		Texture* m_DepthTexture;
	};

}