#include "agf/renderer/Framebuffer.h"
#include "agf/renderer/Texture.h"

#include <GL/glew.h>

#include <iostream>

namespace agf {

	Framebuffer::Framebuffer(unsigned width, unsigned height, const std::vector<Texture*>& colorTextures, Texture* depthTexture, unsigned layer)
		: m_Width(width), m_Height(height), m_ColorTextures(colorTextures), m_DepthTexture(depthTexture), m_DepthRenderbufferHandle(0)
	{
		glGenFramebuffers(1, &m_Handle);
		glBindFramebuffer(GL_FRAMEBUFFER, m_Handle);

		if (!m_ColorTextures.empty())
		{
			std::vector<unsigned> drawBuffers;
			for (unsigned i = 0; i < m_ColorTextures.size(); ++i)
			{
				Texture* colorTexture = m_ColorTextures[i];
				unsigned drawBuffer = GL_COLOR_ATTACHMENT0 + i;
				if (colorTexture->GetDepth() > 1)
					glFramebufferTextureLayer(GL_FRAMEBUFFER, drawBuffer, colorTexture->GetHandle(), 0, layer);
				else
					glFramebufferTexture(GL_FRAMEBUFFER, drawBuffer, colorTexture->GetHandle(), 0);
				drawBuffers.push_back(drawBuffer);
			}

			if (!drawBuffers.empty())
				glDrawBuffers(static_cast<GLsizei>(drawBuffers.size()), &drawBuffers[0]);
		}
		else
		{
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);
		}

		if (m_DepthTexture)
			if (m_DepthTexture->GetDepth() > 1)
				glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_DepthTexture->GetHandle(), 0, layer);
			else
				glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_DepthTexture->GetHandle(), 0);
		else
		{
			glGenRenderbuffers(1, &m_DepthRenderbufferHandle);
			glBindRenderbuffer(GL_RENDERBUFFER, m_DepthRenderbufferHandle);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_Width, m_Height);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthRenderbufferHandle);
		}

		int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer incomplete" << std::endl;

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	Framebuffer::~Framebuffer()
	{
		if (!m_DepthTexture)
			glDeleteRenderbuffers(1, &m_DepthRenderbufferHandle);

		glDeleteFramebuffers(1, &m_Handle);
	}

	void Framebuffer::Bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_Handle);
		glViewport(0, 0, m_Width, m_Height);
		unsigned clearMask = 0;
		if (!m_ColorTextures.empty())
			clearMask |= GL_COLOR_BUFFER_BIT;
		if (m_DepthTexture)
			clearMask |= GL_DEPTH_BUFFER_BIT;
		glClear(clearMask);
	}

	void Framebuffer::Unbind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

}