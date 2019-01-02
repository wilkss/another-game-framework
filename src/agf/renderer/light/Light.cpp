#include "agf/renderer/light/Light.h"
#include "agf/renderer/Texture.h"

namespace agf {

	Light::Light(const glm::vec3& color, unsigned shadowMapSize)
		: m_Color(color), m_ShadowMapSize(shadowMapSize)
	{
	}

	Light::~Light()
	{
		delete m_ShadowMapTexture;
	}

}