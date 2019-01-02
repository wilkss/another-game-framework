#include "agf/renderer/MeshRenderer.h"
#include "agf/renderer/ShaderProgram.h"
#include "agf/renderer/Mesh.h"
#include "agf/Entity.h"

namespace agf {

	MeshRenderer::MeshRenderer(Mesh* mesh, unsigned materialID)
		: m_Mesh(mesh), m_MaterialID(materialID)
	{
	}

	void MeshRenderer::Render(ShaderProgram* shader)
	{
		shader->SetMat4("u_ModelMatrix", m_Parent->GetModelMatrix());
		m_Mesh->Render();
	}

}