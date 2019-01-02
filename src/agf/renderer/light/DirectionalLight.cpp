#include "agf/renderer/light/DirectionalLight.h"
#include "agf/renderer/Framebuffer.h"
#include "agf/renderer/Texture.h"
#include "agf/renderer/Camera.h"
#include "agf/renderer/ShaderProgram.h"
#include "agf/renderer/Mesh.h"
#include "agf/renderer/MeshRenderer.h"
#include "agf/Scene.h"
#include "agf/Entity.h"

#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

namespace agf {

	DirectionalLight::DirectionalLight(const glm::vec3& color, unsigned shadowMapSize, unsigned shadowCascadeCount)
		: Light(color, shadowMapSize), m_ShadowCascadeCount(shadowCascadeCount > MAX_SHADOW_CASCADES ? MAX_SHADOW_CASCADES : shadowCascadeCount)
	{
		m_ShadowMapTexture = new Texture(m_ShadowMapSize, m_ShadowMapSize, GL_TEXTURE_2D_ARRAY, GL_DEPTH_COMPONENT32, GL_DEPTH_COMPONENT, GL_FLOAT, GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE, m_ShadowCascadeCount);

		for (unsigned i = 0; i < m_ShadowCascadeCount; ++i)
			m_ShadowMapFramebuffers[i] = new Framebuffer(m_ShadowMapSize, m_ShadowMapSize, {}, m_ShadowMapTexture, i);
	}

	DirectionalLight::~DirectionalLight()
	{
		for (unsigned i = 0; i < m_ShadowCascadeCount; ++i)
			delete m_ShadowMapFramebuffers[i];
	}

	// https://johanmedestrom.wordpress.com/2016/03/18/opengl-cascaded-shadow-maps/
	void DirectionalLight::Update(Camera* camera)
	{
		float cascadeSplits[MAX_SHADOW_CASCADES];

		float nearClip = camera->GetNearClip();
		float farClip = camera->GetFarClip();
		float clipRange = farClip - nearClip;

		float minZ = nearClip;
		float maxZ = nearClip + clipRange;

		float range = maxZ - minZ;
		float ratio = maxZ / minZ;

		// https://developer.nvidia.com/gpugems/GPUGems3/gpugems3_ch10.html
		for (unsigned i = 0; i < m_ShadowCascadeCount; ++i)
		{
			float p = (i + 1) / static_cast<GLfloat>(m_ShadowCascadeCount);
			float log = minZ * std::pow(ratio, p);
			float uniform = minZ + range * p;
			float d = m_CascadeSplitLambda * (log - uniform) + uniform;
			cascadeSplits[i] = (d - nearClip) / clipRange;
		}

		float prevSplitDistance = 0.0;
		for (unsigned i = 0; i < m_ShadowCascadeCount; ++i) {
			float splitDistance = cascadeSplits[i];

			glm::vec3 frustumCornersWS[8] =
			{
				glm::vec3(-1.0f, 1.0f, -1.0f),
				glm::vec3(1.0f, 1.0f, -1.0f),
				glm::vec3(1.0f, -1.0f, -1.0f),
				glm::vec3(-1.0f, -1.0f, -1.0f),
				glm::vec3(-1.0f, 1.0f, 1.0f),
				glm::vec3(1.0f, 1.0f, 1.0f),
				glm::vec3(1.0f, -1.0f, 1.0f),
				glm::vec3(-1.0f, -1.0f, 1.0f),
			};

			glm::mat4 invCam = glm::inverse(camera->GetProjectionMatrix() * camera->GetViewMatrix());
			for (unsigned i = 0; i < 8; ++i)
			{
				glm::vec4 inversePoint = invCam * glm::vec4(frustumCornersWS[i], 1.0f);
				frustumCornersWS[i] = glm::vec3(inversePoint / inversePoint.w);
			}

			for (unsigned i = 0; i < 4; ++i)
			{
				glm::vec3 cornerRay = frustumCornersWS[i + 4] - frustumCornersWS[i];
				glm::vec3 nearCornerRay = cornerRay * prevSplitDistance;
				glm::vec3 farCornerRay = cornerRay * splitDistance;
				frustumCornersWS[i + 4] = frustumCornersWS[i] + farCornerRay;
				frustumCornersWS[i] = frustumCornersWS[i] + nearCornerRay;
			}

			glm::vec3 frustumCenter = glm::vec3(0.0f);
			for (unsigned i = 0; i < 8; ++i)
				frustumCenter += frustumCornersWS[i];
			frustumCenter /= 8.0f;

			float radius = 0.0f;
			for (unsigned i = 0; i < 8; ++i)
			{
				float distance = glm::length(frustumCornersWS[i] - frustumCenter);
				radius = glm::max(radius, distance);
			}
			radius = std::ceil(radius * 16.0f) / 16.0f;

			glm::vec3 maxExtents = glm::vec3(radius);
			glm::vec3 minExtents = -maxExtents;

			glm::vec3 lightDir = normalize(m_Parent->GetRotation());
			glm::mat4 lightViewMatrix = glm::lookAt(frustumCenter - lightDir * -minExtents.z, frustumCenter, glm::vec3(0.0f, 1.0f, 0.0f));
			glm::mat4 lightOrthoMatrix = glm::ortho(minExtents.x, maxExtents.x, minExtents.y, maxExtents.y, 0.0f, maxExtents.z - minExtents.z);

			m_SplitDepths[i] = (camera->GetNearClip() + splitDistance * clipRange) * -1.0f;
			m_ShadowProjectionMatrices[i] = lightOrthoMatrix;
			m_ShadowViewMatrices[i] = lightViewMatrix;

			prevSplitDistance = cascadeSplits[i];
		}
	}

}