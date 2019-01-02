#include "agf/Application.h"
#include "agf/Window.h"
#include "agf/Entity.h"
#include "agf/Scene.h"
#include "agf/UpdateableComponent.h"
#include "agf/renderer/Renderer.h"
#include "agf/renderer/Material.h"
#include "agf/renderer/Mesh.h"
#include "agf/renderer/MeshFactory.h"
#include "agf/renderer/MeshRenderer.h"
#include "agf/renderer/ShaderProgram.h"
#include "agf/renderer/Camera.h"
#include "agf/renderer/Texture.h"
#include "agf/renderer/light/DirectionalLight.h"
#include "agf/renderer/light/PointLight.h"

#include <glm/gtc/matrix_transform.hpp>

class FlyCameraControllerComponent : public agf::UpdateableComponent
{
public:
	FlyCameraControllerComponent(agf::Window* window)
		: m_Window(window)
	{
		m_Velocity = glm::vec2(0.0f);
	}

	void Update(agf::Camera* camera) override
	{
		glm::vec2 mouseDelta = m_Window->GetCusorPosition() - m_LastCursorPos;

		if (m_Window->IsMouseButtonPressed(AGF_MOUSE_BUTTON_RIGHT))
		{
			auto mouseSensitivity = 0.1f;
			m_Parent->SetRotation(m_Parent->GetRotation() - glm::vec3(mouseDelta.y * mouseSensitivity, mouseDelta.x * mouseSensitivity, 0.0f));

			auto forward = camera->GetForwardVector();
			auto up = camera->GetUpVector();
			auto right = camera->GetRightVector(forward, up);

			auto walkSpeed = 0.05f;
			auto sprintSpeed = 0.25f;
			auto currentSpeed = m_Window->IsKeyPressed(AGF_KEY_LEFT_SHIFT) ? sprintSpeed : walkSpeed;

			auto velocity = glm::vec2(0);

			if (m_Window->IsKeyPressed(AGF_KEY_W))
				velocity.x += currentSpeed;
			else if (m_Window->IsKeyPressed(AGF_KEY_S))
				velocity.x -= currentSpeed;

			if (m_Window->IsKeyPressed(AGF_KEY_A))
				velocity.y -= currentSpeed;
			else if (m_Window->IsKeyPressed(AGF_KEY_D))
				velocity.y += currentSpeed;

			if (velocity.x == 0)
				m_Velocity.x = 0;
			if (velocity.y == 0)
				m_Velocity.y = 0;

			m_Parent->SetPosition(m_Parent->GetPosition() + (forward * m_Velocity.x));
			m_Parent->SetPosition(m_Parent->GetPosition() + (right * m_Velocity.y));
			m_Velocity += velocity;

			glm::mat4 viewMatrix(1.0f);
			viewMatrix = glm::rotate(viewMatrix, glm::radians(-m_Parent->GetRotation().x), glm::vec3(1, 0, 0));
			viewMatrix = glm::rotate(viewMatrix, glm::radians(-m_Parent->GetRotation().y), glm::vec3(0, 1, 0));
			viewMatrix = glm::translate(viewMatrix, -m_Parent->GetPosition());
			camera->SetViewMatrix(viewMatrix);
		}

		m_LastCursorPos = m_Window->GetCusorPosition();
	}

private:
	agf::Window* m_Window;
	glm::vec2 m_LastCursorPos;
	glm::vec2 m_Velocity;
};

class SpinComponent : public agf::UpdateableComponent
{
public:
	void Update(agf::Camera* camera) override
	{
		rot++;
		if (rot >= 360.0f) rot = 0.0f;
		m_Parent->SetRotation({ 0.0f, rot, 0.0f });
	}

private:
	float rot;
};

class DemoApplication : public agf::Application
{
public:
	DemoApplication()
		: agf::Application("Another Game Framework Demo", 800, 600)
	{
		agf::Entity* mainCamera = new agf::Entity("Main Camera");
		mainCamera->AddComponent(new agf::Camera(65.0f, m_Window->GetWidth() / static_cast<float>(m_Window->GetHeight()), 0.1f, 1024.0f));
		mainCamera->AddComponent(new FlyCameraControllerComponent(m_Window));
		m_Scene->AddEntity(mainCamera);

		m_UnitCubeMesh = agf::MeshFactory::CreateCubeMesh(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.5f, 0.5f, 0.5f));

		//agf::Entity* sunLight = new agf::Entity("Sun Light", glm::vec3(0.0f), glm::vec3(-1.0f, -1.0f, 0.0f));
		//sunLight->AddComponent(new agf::DirectionalLight(glm::vec3(1.0f), 2048, 4));
		//m_Scene->AddEntity(sunLight);

		agf::Entity* pointLight1 = new agf::Entity("Point Light 1", glm::vec3(0.0f, 1.2f, -8.0f));
		pointLight1->AddComponent(new agf::PointLight(glm::vec3(1.0f), 2048, { 1.0f, 0.09f, 0.032f }));
		m_Scene->AddEntity(pointLight1);

		m_FloorCubeMesh = agf::MeshFactory::CreateCubeMesh(glm::vec3(-8.0f, -0.5f, -8.0f), glm::vec3(8.0f, 0.5f, 8.0f));
		agf::MeshRenderer* floorCubeRenderComponent = new agf::MeshRenderer(m_FloorCubeMesh);
		agf::Entity* floorCube = new agf::Entity("Floor", glm::vec3(0.0f, -2.0f, -8.0f));
		floorCube->AddComponent(floorCubeRenderComponent);
		m_Scene->AddEntity(floorCube);

		for (unsigned i = 0; i < 8; ++i)
		{
			float x = sinf((float)i / 8.0f * 3.14159f * 2.0f) * 4.0f;
			float z = cosf((float)i / 8.0f * 3.14159f * 2.0f) * 4.0f;
			agf::MeshRenderer* unitCubeRenderComponent = new agf::MeshRenderer(m_UnitCubeMesh);
			SpinComponent* spinComponent = new SpinComponent();
			agf::Entity* unitCube = new agf::Entity("Spinning Cube " + std::to_string(i + 1), glm::vec3(x, 0.0f, -8.0f + z));
			unitCube->AddComponent(unitCubeRenderComponent);
			unitCube->AddComponent(spinComponent);
			m_Scene->AddEntity(unitCube);
		}
	}

	~DemoApplication()
	{
		delete m_UnitCubeMesh;
		delete m_FloorCubeMesh;
	}

private:
	agf::Mesh* m_UnitCubeMesh;
	agf::Mesh* m_FloorCubeMesh;
};

AGF_ENTRY_POINT(DemoApplication)