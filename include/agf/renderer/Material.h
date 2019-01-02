#pragma once

#include <glm/glm.hpp>

#include <unordered_map>

namespace agf {

	class Texture;

	class Material
	{
	public:
		Material(const glm::vec3& albedo = glm::vec3(1.0f))
			: m_Albedo(albedo), m_AlbedoMap(nullptr)
		{
		}

		inline void SetAlbedo(const glm::vec3& albedo) { m_Albedo = albedo; }
		inline const glm::vec3& GetAlbedo() const { return m_Albedo; }
		inline void SetAlbedoMap(Texture* albedoMap) { m_AlbedoMap = albedoMap; }
		inline bool HasAlbedoMap() const { return m_AlbedoMap; }
		inline Texture* GetAlbedoMap() const { return m_AlbedoMap; }

	private:
		glm::vec3 m_Albedo;
		Texture* m_AlbedoMap;
	};

}