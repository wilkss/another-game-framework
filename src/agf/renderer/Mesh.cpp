#include "agf/renderer/Mesh.h"
#include "agf/renderer/IndexBuffer.h"
#include "agf/renderer/VertexArray.h"

#include <GL/glew.h>

namespace agf {

	Mesh::Mesh(VertexArray* vertexArray, IndexBuffer* indexBuffer)
		: m_VertexArray(vertexArray), m_IndexBuffer(indexBuffer)
	{
	}

	Mesh::~Mesh()
	{
		delete m_VertexArray;
		delete m_IndexBuffer;
	}

	void Mesh::Render()
	{
		m_VertexArray->Bind();
		m_IndexBuffer->Bind();
		glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);
		m_IndexBuffer->Unbind();
		m_VertexArray->Unbind();
	}

}