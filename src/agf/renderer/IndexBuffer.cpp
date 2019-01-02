#include "agf/renderer/IndexBuffer.h"

#include <GL/glew.h>

namespace agf {

	IndexBuffer::IndexBuffer(const unsigned* data, unsigned count)
		: m_Count(count)
	{
		glGenBuffers(1, &m_Handle);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Handle);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned), data, GL_STATIC_DRAW);
	}

	IndexBuffer::~IndexBuffer()
	{
		glDeleteBuffers(1, &m_Handle);
	}

	void IndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Handle);
	}

	void IndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

}