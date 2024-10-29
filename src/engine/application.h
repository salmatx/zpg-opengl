#pragma once
#include "camera.h"
#include "scene.h"
#include "shader.h"

namespace engine {

class Application {
public:
	Application(int t_scr_width, int t_scr_height, std::string t_title);
	~Application() = default;
	void LinkShader(const std::string& t_name, const std::string& t_path);
	void SetShader(Scene& t_scene, const std::string& t_name);
	void CreateCamera(const CameraPosition& t_position, const CameraDepth& t_depth);
	Scene CreateScene();
	bool Run();

private:
	std::unique_ptr<Window> m_window {};
	std::unique_ptr<Camera> m_camera {};
	std::unordered_map<std::string, std::string> m_shader_paths;
	std::unordered_map<std::string, std::shared_ptr<Shader>> m_shaders;
	int m_scr_width;
	int m_scr_height;
};

}
