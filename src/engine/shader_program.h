#pragma once

#include <shader_loader.h>

#include "camera.h"
#include "i_camera_observer.h"
#include "i_shader.h"

namespace engine {
class ShaderProgram : public IShader, public ICameraObserver {
public:
	ShaderProgram(Camera& t_camera);
	ShaderProgram(const ShaderProgram& t_other) = delete;
	ShaderProgram& operator=(const ShaderProgram& t_other) = delete;
	~ShaderProgram();

	void Bind() const override;
	void Unbind() const override;
	void SetUniform4f(const std::string& t_name, float t_v0, float t_v1, float t_v2, float t_v3) override;
	void SetUniformMat4f(const std::string& t_name, const glm::mat4& t_matrix) override;
	void LoadShader(const std::string& t_filepath);
	void CreateShaderProgram();

	void Update(const glm::mat4& t_projection, const glm::mat4& t_view) override;

	void RemoveObservation();

private:
	int GetUniformLocation(const std::string& t_name);
	std::optional<unsigned int> CompileShaderProgram(unsigned int t_type, const std::string& t_source);

	Camera* m_camera;
	unsigned int m_rendered_ID {0};
	std::unordered_map<std::string, int> m_uniform_location_cache;
	ShaderLoader m_loader;
	std::vector<Shader> m_shaders;
};

}
