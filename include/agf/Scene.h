#pragma once

#include <vector>

namespace agf {

	class Camera;
	class Entity;
	class Light;
	class MeshRenderComponent;

	class Scene
	{
	public:
		~Scene();

		void Update();
		Camera* GetMainCamera();

		template <typename T>
		std::vector<T*> GetComponentsOfType()
		{
			std::vector<T*> result;

			for (auto* entity : m_Entities)
			{
				for (auto* c : entity->GetComponents())
				{
					T* component = dynamic_cast<T*>(c);
					if (component)
						result.push_back(component);
				}
			}

			return result;
		}

		inline void AddEntity(Entity* entity) { m_Entities.push_back(entity); }
		inline const std::vector<Entity*>& GetEntities() const { return m_Entities; }

	private:
		Camera* m_MainCamera;
		std::vector<Entity*> m_Entities;
	};

}