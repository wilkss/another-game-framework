#pragma once

#include "agf/renderer/light/Light.h"

namespace agf {

	struct Attenuation
	{
		float constant;
		float linear;
		float quadratic;
	};

	class Framebuffer;

	class PointLight : public Light
	{
	public:
		PointLight(const glm::vec3& color, unsigned shadowMapSize, const Attenuation& attenuation);
		~PointLight();

		void Update(Camera* camera) override;

		inline const Attenuation& GetAttenuation() const { return m_Attenuation; }
		inline float GetShadowDistance() const { return m_ShadowDistance; }
		inline Framebuffer* GetShadowFramebuffer() const { return m_ShadowFramebuffer; }
		inline const glm::mat4& GetShadowMapProjectionMatrix() const { return m_ShadowMapProjectionMatrix; }
		inline const glm::mat4& GetShadowMapViewMatrix(unsigned f) const { return m_ShadowMapViewMatrices[f]; }

	private:
		Attenuation m_Attenuation;

		float m_ShadowDistance = 25.0f;
		Framebuffer* m_ShadowFramebuffer;
		glm::mat4 m_ShadowMapProjectionMatrix;
		glm::mat4 m_ShadowMapViewMatrices[6];
	};

}