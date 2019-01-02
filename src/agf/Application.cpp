#include "agf/Application.h"
#include "agf/Window.h"
#include "agf/Scene.h"
#include "agf/renderer/Renderer.h"

namespace agf {

	Application::Application(const std::string& name, unsigned windowWidth, unsigned windowHeight)
		: m_Running(true), m_Name(name), m_Window(nullptr), m_Renderer(nullptr), m_Scene(nullptr)
	{
		m_Window = new Window(windowWidth, windowHeight, name);
		m_Scene = new Scene();
		m_Renderer = new Renderer(m_Window, m_Scene);
	}

	Application::~Application()
	{
		delete m_Window;
		delete m_Renderer;
		delete m_Scene;
	}

	void Application::Start()
	{
		while (m_Running && !m_Window->ShouldClose())
		{
			if (m_Scene)
				m_Scene->Update();

			if (m_Renderer)
				m_Renderer->Render();

			if (m_Window)
			{
				m_Window->SwapBuffers();
				m_Window->PollEvents();
			}
		}
	}
	
	void Application::Stop()
	{
		m_Running = false;
	}

}