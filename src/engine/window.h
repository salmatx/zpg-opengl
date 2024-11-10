#pragma once
#include "i_window_subject.h"

namespace engine {

class Window : public IWindowSubject {
public:
	Window(int t_width, int t_height, std::string t_title);
	~Window();

	void Attach(EventType t_eventType, IWindowObserver* t_observer) override;
	void Detach(EventType t_eventType, IWindowObserver* t_observer) override;

	void SetKeyCallbacks() const;
	void PollEvents() const;
	void SwapBuffers() const;
	bool RenderLoop() const;

	void PrintInfo() const;

private:
	static void ErrorCallback(int t_error, const char* t_description);
	static void KeyCallback(GLFWwindow* t_window, int t_key, int t_scancode, int t_action, int t_mods);
	static void ScrollCallback(GLFWwindow* t_window, double t_xoffset, double t_yoffset);
	static void WindowFocusCallback(GLFWwindow* t_window, int t_focused);
	static void WindowIconifyCallback(GLFWwindow* t_window, int t_iconified);
	static void WindowSizeCallback(GLFWwindow* t_window, int t_width, int t_height);
	static void CursorCallback(GLFWwindow* t_window, double t_x, double t_y);
	static void ButtonCallback(GLFWwindow* t_window, int t_button, int t_action, int t_mode);
	static void GetTime();

	void Notify(EventType t_event) const override;
	void InitGlew() const;
	void CreateWindow(int t_width, int t_height, const std::string& t_title);

	std::unordered_map<EventType, IWindowObserver*> m_observers;
	GLFWwindow* m_window;

	static float m_time_per_frame;
	static float m_last_frame;
	static std::shared_ptr<const void> m_event;
};

}
