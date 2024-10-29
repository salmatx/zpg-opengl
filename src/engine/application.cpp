#include "epch.h"
#include "application.h"

#include <utility>

namespace engine {
Application::Application(int t_scr_width, int t_scr_height, std::string t_title)
	: m_window(new Window(t_scr_width, t_scr_height, std::move(t_title))),
	m_scr_width(t_scr_width),
	m_scr_height(t_scr_height) {
	m_window->SetKeyCallbacks();
}

void Application::LinkShader(const std::string& t_name, const std::string& t_path) {
	m_shader_paths[t_name] = t_path;
}

void Application::SetShader(Scene& t_scene, const std::string& t_name) {
	if (m_camera == nullptr) {
		std::cerr << "Create camera first" << std::endl;
		return;
	}

	auto it = m_shaders.find(t_name);
	if (it != m_shaders.end()) {
		t_scene.SetShader(it->second);
		return;
	}

	auto path_iter = m_shader_paths.find(t_name);
	if (path_iter != m_shader_paths.end()) {
		auto shader = std::make_shared<Shader>(*m_camera, path_iter->second);
		m_shaders[t_name] = shader;
		t_scene.SetShader(shader);
		m_camera->InitCamera();
	}
}

void Application::CreateCamera(const CameraPosition& t_position, const CameraDepth& t_depth) {
	m_camera = std::make_unique<Camera>(*m_window, m_scr_width, m_scr_height, t_depth, t_position);
}

Scene Application::CreateScene() {
	return Scene{};
}

bool Application::Run() {
	m_window->PollEvents();
	m_window->SwapBuffers();
	return m_window->RenderLoop();
}
}
