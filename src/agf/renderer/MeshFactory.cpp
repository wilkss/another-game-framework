#include "agf/renderer/MeshFactory.h"
#include "agf/renderer/Mesh.h"
#include "agf/renderer/VertexBuffer.h"
#include "agf/renderer/IndexBuffer.h"
#include "agf/renderer/VertexArray.h"

#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

namespace agf {

	Mesh* MeshFactory::CreateSphereMesh(float radius, unsigned slices, unsigned stacks)
	{
		std::vector<float> vertices;
		std::vector<unsigned> indices;

		for (unsigned i = 0; i <= stacks; ++i) {
			float v = static_cast<float>(i) / stacks;
			float phi = glm::pi<float>() * v;

			for (unsigned j = 0; j <= slices; j++) {
				float u = static_cast<float>(j) / slices;
				float theta = 2 * glm::pi<float>() * u;

				glm::vec3 position(cosf(theta) * sinf(phi), cosf(phi), sinf(theta) * sinf(phi));

				vertices.push_back(position.x * radius);
				vertices.push_back(position.y * radius);
				vertices.push_back(position.z * radius);
				vertices.push_back(u);
				vertices.push_back(v);
				vertices.push_back(position.x);
				vertices.push_back(position.y);
				vertices.push_back(position.z);
			}
		}

		for (unsigned i = 0; i < stacks * slices + slices; ++i)
		{
			indices.push_back(i);
			indices.push_back(i + slices + 1);
			indices.push_back(i + slices);

			indices.push_back(i + slices + 1);
			indices.push_back(i);
			indices.push_back(i + 1);
		}

		VertexArray* vertexArray = new VertexArray();
		vertexArray->Bind();
		VertexBuffer vertexBuffer(&vertices[0], vertices.size() * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(2);
		layout.Push<float>(3);
		vertexArray->SetBufferLayout(layout);
		IndexBuffer* indexBuffer = new IndexBuffer(&indices[0], indices.size());
		vertexArray->Unbind();
		return new Mesh(vertexArray, indexBuffer);
	}

	Mesh* MeshFactory::CreateCubeMesh(const glm::vec3& minExtents, const glm::vec3& maxExtents)
	{
		float vertices[] = {
			minExtents.x, maxExtents.y, minExtents.z, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
			maxExtents.x, maxExtents.y, minExtents.z, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
			maxExtents.x, minExtents.y, minExtents.z, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
			minExtents.x, minExtents.y, minExtents.z, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f,
			maxExtents.x, maxExtents.y, maxExtents.z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
			minExtents.x, maxExtents.y, maxExtents.z, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
			minExtents.x, minExtents.y, maxExtents.z, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			maxExtents.x, minExtents.y, maxExtents.z, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			maxExtents.x, maxExtents.y, minExtents.z, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
			maxExtents.x, maxExtents.y, maxExtents.z, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
			maxExtents.x, minExtents.y, maxExtents.z, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
			maxExtents.x, minExtents.y, minExtents.z, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
			minExtents.x, maxExtents.y, maxExtents.z, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
			minExtents.x, maxExtents.y, minExtents.z, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
			minExtents.x, minExtents.y, minExtents.z, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
			minExtents.x, minExtents.y, maxExtents.z, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
			minExtents.x, maxExtents.y, maxExtents.z, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
			maxExtents.x, maxExtents.y, maxExtents.z, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
			maxExtents.x, maxExtents.y, minExtents.z, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
			minExtents.x, maxExtents.y, minExtents.z, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
			minExtents.x, minExtents.y, minExtents.z, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
			maxExtents.x, minExtents.y, minExtents.z, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
			maxExtents.x, minExtents.y, maxExtents.z, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
			minExtents.x, minExtents.y, maxExtents.z, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f
		};

		unsigned indices[] = {
			0, 1, 2, 2, 3, 0,
			4, 5, 6, 6, 7, 4,
			8, 9, 10, 10, 11, 8,
			12, 13, 14, 14, 15, 12,
			16, 17, 18, 18, 19, 16,
			20, 21, 22, 22, 23, 20,
		};

		VertexArray* vertexArray = new VertexArray();
		vertexArray->Bind();
		VertexBuffer vertexBuffer(vertices, 8 * 24 * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(2);
		layout.Push<float>(3);
		vertexArray->SetBufferLayout(layout);
		IndexBuffer* indexBuffer = new IndexBuffer(indices, 6 * 6);
		vertexArray->Unbind();
		return new Mesh(vertexArray, indexBuffer);
	}

	Mesh* MeshFactory::CreateScreenSpaceQuadMesh()
	{
		float vertices[] = {
			-1.0f, -1.0f, 0.0f, 0.0f,
			1.0f, -1.0f, 1.0f, 0.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f, 0.0f, 1.0f
		};

		unsigned indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		VertexArray* vertexArray = new VertexArray();
		vertexArray->Bind();
		VertexBuffer vertexBuffer(vertices, 4 * 4 * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		vertexArray->SetBufferLayout(layout);
		IndexBuffer* indexBuffer = new IndexBuffer(indices, 6);
		vertexArray->Unbind();
		return new Mesh(vertexArray, indexBuffer);
	}

}