#include "agf/Entity.h"
#include "agf/Component.h"

#include <glm/gtc/matrix_transform.hpp>

namespace agf {

	Entity::Entity(const std::string& name, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
		: m_Name(name), m_Position(position), m_Rotation(rotation), m_Scale(scale), m_TransformationDirty(true)
	{
	}

	Entity::~Entity()
	{
		for (auto component : m_Components)
			delete component;
	}

	void Entity::UpdateModelMatrix()
	{
		glm::mat4 modelMatrix(1.0f);
		modelMatrix = glm::translate(modelMatrix, m_Position);
		modelMatrix = glm::rotate(modelMatrix, glm::radians(m_Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(m_Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(m_Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		modelMatrix = glm::scale(modelMatrix, m_Scale);
		m_ModelMatrix = modelMatrix;
	}

	void Entity::AddComponent(Component* component)
	{
		m_Components.push_back(component);
		component->SetParent(this);
	}

}