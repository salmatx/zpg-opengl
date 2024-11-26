#pragma once
#include "camera.h"
#include "scene.h"
#include "shader_program.h"

namespace engine {

class Application {
public:
	Application(int t_scr_width, int t_scr_height, std::string t_title);
	~Application() = default;
	void UseShaderProgram(Scene& t_scene, const std::string& t_name);
	void CreateCamera(const CameraPosition& t_position, const CameraDepth& t_depth);
	Scene CreateScene();
	Light CreateLight(const LightParams& t_params);
	void UseLight(const Light& t_light);
	bool Run();

	template<typename... Args>
	void CreateShaderProgram(const std::string& t_name, Args&&... t_paths) {
		(m_shader_paths[t_name].emplace_back(std::forward<Args>(t_paths)), ...);
	}

private:
	std::unique_ptr<Window> m_window {};
	std::unique_ptr<Camera> m_camera {};
	std::shared_ptr<Light> m_light;
	std::unordered_map<std::string, std::vector<std::string>> m_shader_paths;
	std::unordered_map<std::string, std::shared_ptr<ShaderProgram>> m_shader_programs;
	int m_scr_width;
	int m_scr_height;
};

}
