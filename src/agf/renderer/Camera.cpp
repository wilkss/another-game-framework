#include "agf/renderer/Camera.h"
#include "agf/Entity.h"

#include <glm/gtc/matrix_transform.hpp>

namespace agf {

	Camera::Camera(float fov, float aspectRatio, float nearClip, float farClip)
		: m_Fov(fov), m_AspectRatio(aspectRatio), m_NearClip(nearClip), m_FarClip(farClip)
	{
		m_ProjectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);
		m_ViewMatrix = glm::mat4(1.0f);
	}

	glm::vec3 Camera::GetForwardVector() const
	{ 
		return glm::vec3(glm::inverse(m_ViewMatrix) * glm::vec4(0, 0, -1, 0)); 
	}

	glm::vec3 Camera::GetUpVector() const
	{
		return glm::vec3(0, 1, 0); 
	}

	glm::vec3 Camera::GetRightVector(const glm::vec3& forward, const glm::vec3& up) const
	{
		return glm::normalize(glm::cross(forward, up));
	}

}