#include "agf/renderer/VertexArray.h"

#include <GL/glew.h>

namespace agf {

	VertexArray::VertexArray()
	{
		glGenVertexArrays(1, &m_Handle);
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &m_Handle);
	}

	void VertexArray::SetBufferLayout(const VertexBufferLayout& layout)
	{
		const auto& elements = layout.GetElements();
		for (unsigned i = 0; i < elements.size(); ++i)
		{
			const auto& element = elements[i];
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i, element.count, element.type, element.normalised, layout.GetStride(), (const void*)element.offset);
		}
	}

	void VertexArray::Bind() const
	{
		glBindVertexArray(m_Handle);
	}

	void VertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

}