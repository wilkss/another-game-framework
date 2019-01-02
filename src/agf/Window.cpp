#include "agf/Window.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace agf {

	Window::Window(unsigned width, unsigned height, const std::string& title)
		: m_Width(width), m_Height(height), m_Title(title)
	{
		if (!glfwInit())
			throw std::runtime_error("Could not initialise GLFW");

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		m_GLFWwindow = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
		if (!m_GLFWwindow)
			throw std::runtime_error("Could not create GLFW window");

		glfwMakeContextCurrent(m_GLFWwindow);
		glfwSwapInterval(1);

		if (glewInit() != GLEW_OK)
			throw std::runtime_error("Could not initialise GLEW");
	}

	Window::~Window()
	{
		if (m_GLFWwindow)
			glfwDestroyWindow(m_GLFWwindow);

		glfwTerminate();
	}

	void Window::SwapBuffers() const
	{
		glfwSwapBuffers(m_GLFWwindow);
	}

	void Window::PollEvents() const
	{
		glfwPollEvents();
	}

	bool Window::IsMouseButtonPressed(unsigned button) const
	{
		return glfwGetMouseButton(m_GLFWwindow, button) == AGF_PRESS;
	}

	bool Window::IsKeyPressed(unsigned key) const
	{
		return glfwGetKey(m_GLFWwindow, key) == AGF_PRESS;
	}

	glm::vec2 Window::GetCusorPosition() const
	{
		double x, y;
		glfwGetCursorPos(m_GLFWwindow, &x, &y);
		return glm::vec2(x, y);
	}

	bool Window::ShouldClose() const
	{
		return glfwWindowShouldClose(m_GLFWwindow);
	}

}