#pragma once

#include "agf/Component.h"

#include <glm/glm.hpp>

namespace agf {
	
	class Camera : public Component
	{
	public:
		Camera(float fov, float aspectRatio, float nearClip, float farClip);

		glm::vec3 GetForwardVector() const;
		glm::vec3 GetUpVector() const;
		glm::vec3 GetRightVector(const glm::vec3& forward, const glm::vec3& up) const;

		inline float GetFov() const { return m_Fov; }
		inline float GetAspectRatio() const { return m_AspectRatio; }
		inline float GetNearClip() const { return m_NearClip; }
		inline float GetFarClip() const { return m_FarClip; }
		inline const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		inline void SetViewMatrix(const glm::mat4& viewMatrix) { m_ViewMatrix = viewMatrix; }
		inline const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }

	protected:
		float m_Fov;
		float m_AspectRatio;
		float m_NearClip;
		float m_FarClip;
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
	};

}