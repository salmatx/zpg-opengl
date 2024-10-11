#include "window.h"

#include <cstdio>
#include <cstdlib>
#include <utility>

namespace engine {
void Window::ErrorCallback(int t_error, const char* t_description) {
	fputs(t_description, stderr);
}

void Window::KeyCallback(GLFWwindow* t_window, int key, int t_scancode, int t_action, int t_mods) {
	if (key == GLFW_KEY_ESCAPE && t_action == GLFW_PRESS)
		glfwSetWindowShouldClose(t_window, GL_TRUE);
	printf("key_callback [%d,%d,%d,%d] \n", key, t_scancode, t_action, t_mods);
}

void Window::WindowFocusCallback(GLFWwindow* t_window, int t_focused) {
	printf("window_focus_callback \n");
}

void Window::WindowIconifyCallback(GLFWwindow* t_window, int t_iconified) {
	printf("window_iconify_callback \n");
}

void Window::WindowSizeCallback(GLFWwindow* t_window, int t_width, int t_height) {
	printf("resize %d, %d \n", t_width, t_height);
	glViewport(0, 0, t_width, t_height);
}

void Window::CursorCallback(GLFWwindow* t_window, double t_x, double t_y) {
	printf("cursor_callback \n");
}

void Window::ButtonCallback(GLFWwindow* t_window, int t_button, int t_action, int t_mode) {
	if (t_action == GLFW_PRESS) printf("button_callback [%d,%d,%d]\n", t_button, t_action, t_mode);
}

Window::Window(int t_width, int t_height, std::string t_title)
	: m_width(t_width), m_height(t_height), m_title(std::move(t_title)) {}

Window::~Window() {
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

void Window::SetKeyCallbacks() const {
	glfwSetKeyCallback(m_window, KeyCallback);
	glfwSetCursorPosCallback(m_window, CursorCallback);
	glfwSetMouseButtonCallback(m_window, ButtonCallback);
	glfwSetWindowFocusCallback(m_window, WindowFocusCallback);
	glfwSetWindowIconifyCallback(m_window, WindowIconifyCallback);
	glfwSetWindowSizeCallback(m_window, WindowSizeCallback);
}

void Window::InitWindow() {
	this->CreateWindow(m_width, m_height, m_title);
	this->InitGlew();

	int width;
	int height;
	glfwGetFramebufferSize(m_window, &width, &height);
	float ratio = width / static_cast<float>(height);
	glViewport(0, 0, width, height);
}

void Window::PollEvents() const {
	glfwPollEvents();
}

void Window::SwapBuffers() const {
	glfwSwapBuffers(m_window);
}

bool Window::RenderLoop() const {
	return !glfwWindowShouldClose(m_window);
}

void Window::InitGlew() const {
	glewExperimental = GL_TRUE;
	glewInit();
}

void Window::CreateWindow(int t_width, int t_height, const std::string& t_title) {
	glfwSetErrorCallback(ErrorCallback);
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		exit(-1);
	}

	m_window = glfwCreateWindow(t_width, t_height, t_title.c_str(), nullptr, nullptr);
	if (!m_window) {
		glfwTerminate();
		exit(-1);
	}

	glfwMakeContextCurrent(m_window);
	glfwSwapInterval(1);
}
}
