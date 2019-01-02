#pragma once

#include "agf/renderer/light/Light.h"

namespace agf {

	class Framebuffer;

	class DirectionalLight : public Light
	{
	public:
		DirectionalLight(const glm::vec3& color, unsigned shadowMapSize, unsigned shadowCascadeCount);
		~DirectionalLight();

		void Update(Camera* camera) override;

		inline unsigned GetShadowCascadeCount() const { return m_ShadowCascadeCount; }
		inline float GetCascadeSplitDepth(unsigned cascade) const { return m_SplitDepths[cascade]; }
		inline const glm::mat4& GetShadowProjectionMatrix(unsigned cascade) const { return m_ShadowProjectionMatrices[cascade]; }
		inline const glm::mat4& GetShadowViewMatrix(unsigned cascade) const { return m_ShadowViewMatrices[cascade]; }
		inline Framebuffer* GetShadowMapFramebuffer(unsigned cascade) const { return m_ShadowMapFramebuffers[cascade]; }

	public:
		constexpr static auto MAX_SHADOW_CASCADES = 4;

	private:
		unsigned m_ShadowCascadeCount;
		Framebuffer* m_ShadowMapFramebuffers[MAX_SHADOW_CASCADES];
		float m_CascadeSplitLambda = 0.95f;
		float m_SplitDepths[MAX_SHADOW_CASCADES];
		glm::mat4 m_ShadowProjectionMatrices[MAX_SHADOW_CASCADES];
		glm::mat4 m_ShadowViewMatrices[MAX_SHADOW_CASCADES];
	};

}