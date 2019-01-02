#include "agf/renderer/light/PointLight.h"
#include "agf/renderer/Texture.h"
#include "agf/renderer/Framebuffer.h"
#include "agf/renderer/Mesh.h"
#include "agf/renderer/MeshRenderer.h"
#include "agf/renderer/ShaderProgram.h"
#include "agf/Scene.h"
#include "agf/Entity.h"

#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>

namespace agf {

	PointLight::PointLight(const glm::vec3& color, unsigned shadowMapSize, const Attenuation& attenuation)
		: Light(color, shadowMapSize), m_Attenuation(attenuation)
	{
		m_ShadowMapProjectionMatrix = glm::perspective(glm::radians(90.0f), static_cast<float>(m_ShadowMapSize) / static_cast<float>(m_ShadowMapSize), 1.0f, m_ShadowDistance);
		m_ShadowMapTexture = new Texture(m_ShadowMapSize, m_ShadowMapSize, GL_TEXTURE_CUBE_MAP, GL_DEPTH_COMPONENT32, GL_DEPTH_COMPONENT, GL_FLOAT, GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE);
		m_ShadowFramebuffer = new Framebuffer(m_ShadowMapSize, m_ShadowMapSize, {}, m_ShadowMapTexture);
	}

	PointLight::~PointLight()
	{
		delete m_ShadowFramebuffer;
	}

	void PointLight::Update(Camera* camera)
	{
		if (m_Parent->IsTransformationDirty())
		{
			m_ShadowMapViewMatrices[0] = glm::lookAt(m_Parent->GetPosition(), m_Parent->GetPosition() + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
			m_ShadowMapViewMatrices[1] = glm::lookAt(m_Parent->GetPosition(), m_Parent->GetPosition() + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
			m_ShadowMapViewMatrices[2] = glm::lookAt(m_Parent->GetPosition(), m_Parent->GetPosition() + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			m_ShadowMapViewMatrices[3] = glm::lookAt(m_Parent->GetPosition(), m_Parent->GetPosition() + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
			m_ShadowMapViewMatrices[4] = glm::lookAt(m_Parent->GetPosition(), m_Parent->GetPosition() + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
			m_ShadowMapViewMatrices[5] = glm::lookAt(m_Parent->GetPosition(), m_Parent->GetPosition() + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		}
	}

}