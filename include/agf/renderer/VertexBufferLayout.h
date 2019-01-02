#pragma once

#include <GL/glew.h>

#include <vector>

namespace agf {

	struct VertexBufferElement
	{
		unsigned type;
		unsigned count;
		unsigned offset;
		bool normalised;
	};

	class VertexBufferLayout
	{
	public:
		template <typename T>
		void Push(unsigned count)
		{
			static_assert(false);
		}

		template <>
		void Push<float>(unsigned count)
		{
			m_Elements.push_back({ GL_FLOAT, count, m_Stride, false });
			m_Stride += count * sizeof(GLfloat);
		}

		template <>
		void Push<unsigned>(unsigned count)
		{
			m_Elements.push_back({ GL_UNSIGNED_INT, count, m_Stride, false });
			m_Stride += count * sizeof(GLuint);
		}

		inline const std::vector<VertexBufferElement>& GetElements() const { return m_Elements; }
		inline unsigned GetStride() const { return m_Stride; }

	private:
		std::vector<VertexBufferElement> m_Elements;
		unsigned m_Stride = 0;
	};

}