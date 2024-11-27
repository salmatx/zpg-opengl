#pragma once

#include "light.h"
#include "shader_loader.h"
#include "camera.h"
#include "i_camera_observer.h"
#include "i_shader.h"

namespace engine {
class ShaderProgram : public IShader, public ICameraObserver, public ILightObserver {
public:
	ShaderProgram(const std::shared_ptr<Camera>& t_camera, const std::vector<std::shared_ptr<Light>>& t_lights);
	ShaderProgram(const ShaderProgram& t_other) = delete;
	ShaderProgram& operator=(const ShaderProgram& t_other) = delete;
	~ShaderProgram();

	void Bind() const override;
	void Unbind() const override;
	void SetUniformMat4f(const std::string& t_name, const glm::mat4& t_matrix) override;
	void LoadShader(const std::string& t_filepath);
	void CreateShaderProgram();

	void Update(const glm::mat4& t_projection, const glm::mat4& t_view,
		const glm::vec3& t_position, const glm::vec3& t_front) override;
	void Update(const DirectionalLight& t_light) override;
	void Update(const PointLight& t_light) override;
	void Update(const FlashLight& t_light, const CameraParams_t& t_camera) override;

	void RemoveObservation();

private:
	int GetUniformLocation(const std::string& t_name);
	std::optional<unsigned int> CompileShaderProgram(unsigned int t_type, const std::string& t_source);
	void SetNumberOfLights();
	void SetUniform4f(const std::string& t_name, float t_v0, float t_v1, float t_v2, float t_v3) override;
	void SetUniform3f(const std::string& t_name, const glm::vec3& t_vector);
	void SetUniform1f(const std::string& t_name, float t_param);
	void SetUniform1i(const std::string& t_name, int t_param);
	void SetIndex(const Light& t_light);

	std::shared_ptr<Camera> m_camera;
	std::vector<std::shared_ptr<Light>> m_lights;
	unsigned int m_rendered_ID {0};
	std::unordered_map<std::string, int> m_uniform_location_cache;
	ShaderLoader m_loader;
	std::vector<Shader> m_shaders;
	std::vector<int> m_dir_lights {};
	std::vector<int> m_point_lights {};
	std::vector<int> m_flash_lights {};
};

}
