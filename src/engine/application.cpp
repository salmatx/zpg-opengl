#include "epch.h"
#include "application.h"

#include <flash_light.h>
#include <point_light.h>
#include <utility>

namespace engine {
Application::Application(int t_scr_width, int t_scr_height, std::string t_title)
	: m_window(std::make_shared<Window>(t_scr_width, t_scr_height, std::move(t_title))),
	m_scr_width(t_scr_width),
	m_scr_height(t_scr_height) {
	m_window->SetKeyCallbacks();
}

void Application::UseShaderProgram(Scene& t_scene, const std::string& t_name, std::shared_ptr<Camera> t_camera,
	std::vector<std::shared_ptr<Light>>t_lights) {
	auto it = m_shader_programs.find(t_name);
	if (it != m_shader_programs.end()) {
		t_scene.SetShaderProgram(it->second);
		return;
	}

	auto path_iter = m_shader_paths.find(t_name);
	if (path_iter != m_shader_paths.end()) {
		auto shader = std::make_shared<ShaderProgram>(t_camera, t_lights);
		for (const auto& path : path_iter->second) {
			shader->LoadShader(path);
		}
		shader->CreateShaderProgram();
		m_shader_programs[t_name] = shader;
		t_scene.SetShaderProgram(shader);
		t_camera->InitCamera();
		for (auto& light : t_lights) {
			light->InitLight();
		}
	}
}

std::shared_ptr<Camera> Application::CreateCamera(const CameraPosition& t_position, const CameraDepth& t_depth) {
	return std::make_shared<Camera>(*m_window, m_scr_width, m_scr_height, t_depth, t_position);
}

Scene Application::CreateScene() {
	return {};
}

std::shared_ptr<Light> Application::CreateDirectionalLight(const DirectionalLightParams_t& t_params) {
	return std::make_shared<DirectionalLight>(t_params);
}

std::shared_ptr<Light> Application::CreatePointLight(const PointLightParams_t& t_params) {
	return std::make_shared<PointLight>(t_params);
}

std::shared_ptr<Light> Application::CreateFlashLight(const FlashLightParams_t& t_params,
	std::shared_ptr<Camera> t_camera) {
	return std::make_shared<FlashLight>(t_params, t_camera);
}

bool Application::Run() {
	m_window->PollEvents();
	m_window->SwapBuffers();
	return m_window->RenderLoop();
}
}
