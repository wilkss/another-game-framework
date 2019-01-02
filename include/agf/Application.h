#pragma once

#include <unordered_map>

namespace agf {

	class Window;
	class Renderer;
	class Scene;

	class Application
	{
	public:
		Application(const std::string& name, unsigned windowWidth, unsigned windowHeight);
		virtual ~Application();

		void Start();
		void Stop();

	protected:
		Window* m_Window;
		Renderer* m_Renderer;
		Scene* m_Scene;

	private:
		bool m_Running;
		std::string m_Name;
	};

}

#define AGF_ENTRY_POINT(applicationClassName) \
	int main() \
	{ \
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); \
		auto* app = new applicationClassName(); \
		app->Start(); \
		delete app; \
	}