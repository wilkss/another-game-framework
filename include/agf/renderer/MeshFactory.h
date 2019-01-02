#pragma once

#include <glm/glm.hpp>

namespace agf {

	class Mesh;

	class MeshFactory
	{
	public:
		static Mesh* CreateSphereMesh(float radius = 1.0f, unsigned slices = 64, unsigned stacks = 64);
		static Mesh* CreateCubeMesh(const glm::vec3& minExtents = glm::vec3(-0.5f), const glm::vec3& maxExtents = glm::vec3(0.5f));
		static Mesh* CreateScreenSpaceQuadMesh();
	};

}