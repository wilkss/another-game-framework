#pragma once

#include "agf/UpdateableComponent.h"

#include <glm/glm.hpp>

namespace agf {

	class Camera;
	class Scene;
	class ShaderProgram;
	class Texture;
	class MeshRenderer;

	class Light : public UpdateableComponent
	{
	public:
		Light(const glm::vec3& color, unsigned shadowMapSize);
		virtual ~Light();

		inline const glm::vec3& GetColor() const { return m_Color; }
		inline Texture* GetShadowMapTexture() const { return m_ShadowMapTexture; }

	protected:
		glm::vec3 m_Color;
		unsigned m_ShadowMapSize;
		Texture* m_ShadowMapTexture;
	};

}