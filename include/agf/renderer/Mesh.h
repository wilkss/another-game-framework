#pragma once

namespace agf {

	class VertexArray;
	class IndexBuffer;

	class Mesh
	{
	public:
		Mesh(VertexArray* vertexArray, IndexBuffer* indexBuffer);
		~Mesh();

		void Render();

	private:
		VertexArray* m_VertexArray;
		IndexBuffer* m_IndexBuffer;
	};

}