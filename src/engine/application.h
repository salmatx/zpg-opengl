#pragma once

#include <directional_light.h>
#include "camera.h"
#include "scene.h"
#include "shader_program.h"

namespace engine {

class Application {
public:
	Application(int t_scr_width, int t_scr_height, std::string t_title);
	~Application() = default;
	std::shared_ptr<ShaderProgram> UseShaderProgram(const std::string& t_name, std::shared_ptr<Camera> t_camera,
		std::vector<std::shared_ptr<Light>> t_lights);
	void RemoveShaderProgram(const std::string& t_name);
	std::shared_ptr<Camera> CreateCamera(const CameraPosition& t_position, const CameraDepth& t_depth);
	Scene CreateScene();
	std::shared_ptr<Light> CreateDirectionalLight(const DirectionalLightParams_t& t_params);
	std::shared_ptr<Light> CreatePointLight(const PointLightParams_t& t_params);
	std::shared_ptr<Light> CreateFlashLight(const FlashLightParams_t& t_params, std::shared_ptr<Camera> t_camera);
	bool Run();

	template<typename... Args>
	void CreateShaderProgram(const std::string& t_name, Args&&... t_paths) {
		(m_shader_paths[t_name].emplace_back(std::forward<Args>(t_paths)), ...);
	}

private:
	std::shared_ptr<Window> m_window {};
	std::unordered_map<std::string, std::vector<std::string>> m_shader_paths;
	std::unordered_map<std::string, std::shared_ptr<ShaderProgram>> m_shader_programs;
	int m_scr_width;
	int m_scr_height;
};

}
