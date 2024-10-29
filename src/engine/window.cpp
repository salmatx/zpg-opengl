#include "epch.h"
#include "window.h"

#include "gl_common.h"

namespace engine {
float Window::m_last_frame = 0.0f;
float Window::m_time_per_frame = 0.0f;

void Window::ErrorCallback(int t_error, const char* t_description) {
	fputs(t_description, stderr);
}

void Window::KeyCallback(GLFWwindow* t_window, int t_key, int t_scancode, int t_action, int t_mods) {
	if (t_key == GLFW_KEY_ESCAPE && t_action == GLFW_PRESS)
		glfwSetWindowShouldClose(t_window, GL_TRUE);
	printf("key_callback [%d,%d,%d,%d] \n", t_key, t_scancode, t_action, t_mods);

	Window* window = static_cast<Window*>(glfwGetWindowUserPointer(t_window));

	auto event = std::make_shared<KeyEvent>(t_key, m_time_per_frame);
	window->Notify(EventType::key, event);
}

void Window::ScrollCallback(GLFWwindow* t_window, double t_xoffset, double t_yoffset) {
	Window* window = static_cast<Window*>(glfwGetWindowUserPointer(t_window));

	auto event = std::make_shared<ScrollEvent>(t_xoffset, t_yoffset);
	window->Notify(EventType::scroll, event);
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

	Window* window = static_cast<Window*>(glfwGetWindowUserPointer(t_window));

	auto event = std::make_shared<ScreenSizeEvent>(t_width, t_height);
	window->Notify(EventType::screen_size, event);
}

void Window::CursorCallback(GLFWwindow* t_window, double t_x, double t_y) {
	printf("cursor_callback \n");

	Window* window = static_cast<Window*>(glfwGetWindowUserPointer(t_window));

	auto event = std::make_shared<MouseEvent>(t_x, t_y);
	window->Notify(EventType::mouse, event);
}

void Window::ButtonCallback(GLFWwindow* t_window, int t_button, int t_action, int t_mode) {
	if (t_action == GLFW_PRESS) printf("button_callback [%d,%d,%d]\n", t_button, t_action, t_mode);
}

Window::Window(int t_width, int t_height, std::string t_title) {
	this->CreateWindow(t_width, t_height, t_title);
	this->InitGlew();

	int width;
	int height;
	glfwGetFramebufferSize(m_window, &width, &height);
	glViewport(0, 0, width, height);
}

Window::~Window() {
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

void Window::Attach(EventType t_eventType, IWindowObserver* t_observer) {
	m_observers[t_eventType] = t_observer;
}

void Window::Detach(EventType t_eventType, IWindowObserver* t_observer) {
	const auto count = std::erase_if(m_observers, [&](const auto& item) {
		const auto& [key, value] = item;
		return key == t_eventType && value == t_observer;
	});

	if (count == 0) {
		std::cerr << "No observers found." << std::endl;
	}
}

void Window::Notify(EventType t_event, std::shared_ptr<const void> t_event_data) const {
	for (const auto& [key, value] : m_observers) {
		if (key == t_event) {
			value->Update(t_event, t_event_data);
		}
	}
}

void Window::SetKeyCallbacks() const {
	glfwSetKeyCallback(m_window, KeyCallback);
	glfwSetCursorPosCallback(m_window, CursorCallback);
	glfwSetMouseButtonCallback(m_window, ButtonCallback);
	glfwSetScrollCallback(m_window, ScrollCallback);
	glfwSetWindowFocusCallback(m_window, WindowFocusCallback);
	glfwSetWindowIconifyCallback(m_window, WindowIconifyCallback);
	glfwSetWindowSizeCallback(m_window, WindowSizeCallback);
}

void Window::PollEvents() const {
	glfwPollEvents();
}

void Window::SwapBuffers() const {
	glfwSwapBuffers(m_window);
}

bool Window::RenderLoop() const {
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	Window::GetTime();
	return !glfwWindowShouldClose(m_window);
}

void Window::PrintInfo() const {
	printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
	printf("Using GLEW %s\n", glewGetString(GLEW_VERSION));
	printf("Vendor %s\n", glGetString(GL_VENDOR));
	printf("Renderer %s\n", glGetString(GL_RENDERER));
	printf("GLSL %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	int major, minor, revision;
	glfwGetVersion(&major, &minor, &revision);
	printf("Using GLFW %i.%i.%i\n", major, minor, revision);
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

	glfwSetWindowUserPointer(m_window, this);

	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Window::GetTime() {
	auto current_frame = static_cast<float>(glfwGetTime());
	m_time_per_frame = current_frame - m_last_frame;
	m_last_frame = current_frame;
}
}
