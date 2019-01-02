#pragma once

#include <glm/glm.hpp>

#include <vector>

namespace agf {

	class Component;

	class Entity
	{
	public:
		Entity(const std::string& name, const glm::vec3& position = glm::vec3(0.0f), const glm::vec3& rotation = glm::vec3(0.0f), const glm::vec3& scale = glm::vec3(1.0f));
		~Entity();

		void AddComponent(Component* component);
		void UpdateModelMatrix();

		inline const glm::mat4& GetModelMatrix()
		{
			if (m_TransformationDirty)
			{
				UpdateModelMatrix();
				m_TransformationDirty = false;
			}

			return m_ModelMatrix;
		}

		inline const std::string& GetName() const { return m_Name; }
		inline void SetPosition(const glm::vec3& position) { m_Position = position; m_TransformationDirty = true; }
		inline const glm::vec3& GetPosition() const { return m_Position; }
		inline void SetRotation(const glm::vec3& rotation) { m_Rotation = rotation; m_TransformationDirty = true; }
		inline const glm::vec3& GetRotation() const { return m_Rotation; }
		inline void SetScale(const glm::vec3& scale) { m_Scale = scale; m_TransformationDirty = true; }
		inline const glm::vec3& GetScale() const { return m_Scale; }
		inline const std::vector<Component*>& GetComponents() const { return m_Components; }
		inline bool IsTransformationDirty() const { return m_TransformationDirty; }

	private:
		std::string m_Name;
		glm::vec3 m_Position;
		glm::vec3 m_Rotation;
		glm::vec3 m_Scale;
		bool m_TransformationDirty;
		std::vector<Component*> m_Components;
		glm::mat4 m_ModelMatrix;
	};

}