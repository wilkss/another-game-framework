#pragma once

#include "agf/Component.h"

namespace agf {

	class Mesh;
	class ShaderProgram;

	class MeshRenderer : public Component
	{
	public:
		MeshRenderer(Mesh* mesh, unsigned materialID = -1);

		void Render(ShaderProgram* shader);

		inline Mesh* GetMesh() const { return m_Mesh; }
		inline unsigned GetMaterialID() const { return m_MaterialID; }

	private:
		Mesh* m_Mesh;
		unsigned m_MaterialID;
	};

}