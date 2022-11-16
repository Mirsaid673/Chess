#include "global.h"
#include "window.h"
#include <spdlog/spdlog.h>

Window::Window(uint32_t width, uint32_t height, const char *title)
{
	create(width, height, title);
}

void Window::create(uint32_t width, uint32_t height, const char *title)
{
	last_size = size = glm::ivec2(width, height);
	m_title = title;
	m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	if (m_window == nullptr)
		spdlog::critical("Failed to create window", title);
}

void Window::destroy()
{
	glfwDestroyWindow(m_window);
}

void Window::makeCurrent()
{
	glfwMakeContextCurrent(m_window);
}

bool Window::shouldClose()
{
	return glfwWindowShouldClose(m_window);
}

void Window::swapBuffers()
{
	glfwSwapBuffers(m_window);
}

void Window::updateSize()
{
	last_size = size;
	glfwGetWindowSize(m_window, (int *)&size.x, (int *)&size.y);
}

void Window::init()
{
	if (!glfwInit())
	{
		spdlog::critical("Failed to initialize GLFW");
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void Window::cleanup()
{
	glfwTerminate();
}