#include "agf/Scene.h"
#include "agf/Entity.h"
#include "agf/UpdateableComponent.h"
#include "agf/renderer/Camera.h"

namespace agf {

	Scene::~Scene()
	{
		for (auto entity : m_Entities)
			delete entity;
	}

	void Scene::Update()
	{
		for (auto component : GetComponentsOfType<UpdateableComponent>())
			component->Update(GetMainCamera());
	}

	Camera* Scene::GetMainCamera()
	{
		std::vector<Camera*> cameras = GetComponentsOfType<Camera>();
		if (cameras.empty())
			return nullptr;

		return cameras[0];
	}

}