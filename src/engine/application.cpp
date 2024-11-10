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

void Application::UseShaderProgram(Scene& t_scene, const std::string& t_name) {
	if (m_camera == nullptr) {
		std::cerr << "Create camera first" << std::endl;
		return;
	}

	if (m_light == nullptr) {
		std::cerr << "Bind light first" << std::endl;
		return;
	}

	auto it = m_shader_programs.find(t_name);
	if (it != m_shader_programs.end()) {
		t_scene.SetShaderProgram(it->second);
		return;
	}

	auto path_iter = m_shader_paths.find(t_name);
	if (path_iter != m_shader_paths.end()) {
		auto shader = std::make_shared<ShaderProgram>(*m_camera, *m_light);
		for (const auto& path : path_iter->second) {
			shader->LoadShader(path);
		}
		shader->CreateShaderProgram();
		m_shader_programs[t_name] = shader;
		t_scene.SetShaderProgram(shader);
		m_camera->InitCamera();
		m_light->InitLight();
	}
}

void Application::CreateCamera(const CameraPosition& t_position, const CameraDepth& t_depth) {
	m_camera = std::make_unique<Camera>(*m_window, m_scr_width, m_scr_height, t_depth, t_position);
}

Scene Application::CreateScene() {
	return {};
}

Light Application::CreateLight(const LightParams& t_params) {
	return {t_params};
}

void Application::UseLight(const Light& t_light) {
	m_light = std::make_shared<Light>(t_light);
}

bool Application::Run() {
	m_window->PollEvents();
	m_window->SwapBuffers();
	return m_window->RenderLoop();
}
}
