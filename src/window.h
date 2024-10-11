#pragma once

#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace engine {

class Window {
public:
	Window(int t_width, int t_height, std::string t_title);
	~Window();

	void SetKeyCallbacks() const;
	void InitWindow();
	void PollEvents() const;
	void SwapBuffers() const;
	bool RenderLoop() const;

private:
	static void ErrorCallback(int t_error, const char* t_description);
	static void KeyCallback(GLFWwindow* t_window, int key, int t_scancode, int t_action, int t_mods);
	static void WindowFocusCallback(GLFWwindow* t_window, int t_focused);
	static void WindowIconifyCallback(GLFWwindow* t_window, int t_iconified);
	static void WindowSizeCallback(GLFWwindow* t_window, int t_width, int t_height);
	static void CursorCallback(GLFWwindow* t_window, double t_x, double t_y);
	static void ButtonCallback(GLFWwindow* t_window, int t_button, int t_action, int t_mode);

	void InitGlew() const;
	void CreateWindow(int t_width, int t_height, const std::string& t_title);

	GLFWwindow* m_window;
	int m_width;
	int m_height;
	std::string m_title;
};

}
