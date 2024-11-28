#include "epch.h"
#include "shader_program.h"

#include <directional_light.h>
#include <flash_light.h>
#include <point_light.h>

#include "gl_common.h"

namespace engine {
ShaderProgram::ShaderProgram(const std::shared_ptr<Camera>& t_camera, const std::vector<std::shared_ptr<Light>>& t_lights)
	: m_camera(t_camera),
	m_lights(t_lights),
	m_loader(ShaderLoader()) {
	m_camera->Attach(this);
	for(auto& light : m_lights) {
		light->Attach(this);
		SetIndex(*light);
	}
}

ShaderProgram::~ShaderProgram() {
	m_camera->Detach(this);
	for(auto& light : m_lights) {
		light->Detach(this);
	}
	GLCall(glDeleteProgram(m_rendered_ID));
}

void ShaderProgram::Bind() const {
	GLCall(glUseProgram(m_rendered_ID));
}

void ShaderProgram::Unbind() const {
	GLCall(glUseProgram(0));
}

void ShaderProgram::SetUniform4f(const std::string& t_name, float t_v0, float t_v1, float t_v2, float t_v3) {
	GLCall(const int location = this->GetUniformLocation(t_name));
	GLCall(glUniform4f(location, t_v0, t_v1, t_v2, t_v3));
}

void ShaderProgram::SetUniform3f(const std::string& t_name, const glm::vec3& t_vector) {
	GLCall(glUniform3f(GetUniformLocation(t_name), t_vector.x, t_vector.y, t_vector.z));
}

void ShaderProgram::SetUniform1f(const std::string& t_name, float t_param) {
	GLCall(glUniform1f(GetUniformLocation(t_name), t_param));
}

void ShaderProgram::SetUniform1i(const std::string& t_name, int t_param) {
	GLCall(glUniform1i(GetUniformLocation(t_name), t_param));
}

void ShaderProgram::SetIndex(const Light& t_light) {
	switch (t_light.GetType()) {
	case LightType_t::Directional:
		m_dir_lights.emplace_back(t_light.GetId());
		break;
	case LightType_t::Point:
		m_point_lights.emplace_back(t_light.GetId());
		break;
	case LightType_t::Flashlight:
		m_flash_lights.emplace_back(t_light.GetId());
		break;
	}
}

void ShaderProgram::SetUniformMat4f(const std::string& t_name, const glm::mat4& t_matrix) {
	GLCall(glUniformMatrix4fv(this->GetUniformLocation(t_name), 1, GL_FALSE, glm::value_ptr(t_matrix)));
}

void ShaderProgram::LoadShader(const std::string& t_filepath) {
	m_shaders.emplace_back(m_loader.LoadShader(t_filepath));
}

void ShaderProgram::Update(const glm::mat4& t_projection, const glm::mat4& t_view,
	const glm::vec3& t_position, const glm::vec3& t_front) {
	Bind();
	SetUniformMat4f("u_project_matrix", t_projection);
	SetUniformMat4f("u_view_matrix", t_view);
	SetUniform3f("u_view_position", t_position);
}

void ShaderProgram::Update(const DirectionalLight& t_light) {
	Bind();

	auto it = std::ranges::find(m_dir_lights, t_light.GetId());
	if (it == m_dir_lights.end()) {
		return;
	}

	const int index = std::distance(m_dir_lights.begin(), it);
	auto [direction, ambient, diffuse, specular] = t_light.GetParams();

	const std::string prefix = "u_dirLights[" + std::to_string(index) + "]";

	SetUniform3f(prefix + ".direction", direction);
	SetUniform3f(prefix + ".ambient", ambient);
	SetUniform3f(prefix + ".diffuse", diffuse);
	SetUniform3f(prefix + ".specular", specular);
}

void ShaderProgram::Update(const PointLight& t_light) {
	Bind();

	auto it = std::ranges::find(m_point_lights, t_light.GetId());
	if (it == m_point_lights.end()) {
		return;
	}

	const int index = std::distance(m_point_lights.begin(), it);
	auto [position, constant, linear, quadratic,
		ambient, diffuse, specular] = t_light.GetParams();

	const std::string prefix = "u_pointLights[" + std::to_string(index) + "]";

	SetUniform3f(prefix + ".position", position);
	SetUniform1f(prefix + ".constant", constant);
	SetUniform1f(prefix + ".linear", linear);
	SetUniform1f(prefix + ".quadratic", quadratic);
	SetUniform3f(prefix + ".ambient", ambient);
	SetUniform3f(prefix + ".diffuse", diffuse);
	SetUniform3f(prefix + ".specular", specular);
}

void ShaderProgram::Update(const FlashLight& t_light, const CameraParams_t& t_camera) {
	Bind();

	auto it = std::ranges::find(m_flash_lights, t_light.GetId());
	if (it == m_flash_lights.end()) {
		return;
	}

	const int index = std::distance(m_flash_lights.begin(), it);
	auto [cut_off_angle, outer_cut_off_angle, constant,linear,
		quadratic,ambient, diffuse, specular] = t_light.GetParams();

	const float cut_off = glm::cos(glm::radians(cut_off_angle));
	const float outer_cut_off = glm::cos(glm::radians(outer_cut_off_angle));

	const std::string prefix = "u_flashLights[" + std::to_string(index) + "]";

	SetUniform3f(prefix + ".position", t_camera.position);
	SetUniform3f(prefix + ".direction", t_camera.direction);
	SetUniform1f(prefix + ".cutOff", cut_off);
	SetUniform1f(prefix + ".outerCutOff", outer_cut_off);
	SetUniform1f(prefix + ".constant", constant);
	SetUniform1f(prefix + ".linear", linear);
	SetUniform1f(prefix + ".quadratic", quadratic);
	SetUniform3f(prefix + ".ambient", ambient);
	SetUniform3f(prefix + ".diffuse", diffuse);
	SetUniform3f(prefix + ".specular", specular);
}

void ShaderProgram::RemoveObservation() {
	m_camera->Detach(this);
	for(auto& light : m_lights) {
		light->Detach(this);
	}
}

int ShaderProgram::GetUniformLocation(const std::string& t_name) {
	if (m_uniform_location_cache.contains(t_name)) {
		return m_uniform_location_cache.at(t_name);
	}

	GLCall(const int location = glGetUniformLocation(m_rendered_ID, t_name.c_str()));
	if (location == -1) {
		std::cerr << "Failed to get uniform location " << t_name << std::endl;
	}
	m_uniform_location_cache[t_name] = location;

	return location;
}

std::optional<unsigned int> ShaderProgram::CompileShaderProgram(const unsigned int t_type, const std::string& t_source) {
	GLCall(unsigned int vertex_shader_ID = glCreateShader(t_type));
	const char* src = t_source.c_str();
	GLCall(glShaderSource(vertex_shader_ID, 1, &src, nullptr));
	GLCall(glCompileShader(vertex_shader_ID));

	int result;
	glGetShaderiv(vertex_shader_ID, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(vertex_shader_ID, GL_INFO_LOG_LENGTH, &length);
		auto message = std::make_unique<char[]>(length);
		glGetShaderInfoLog(vertex_shader_ID, length, &length, message.get());
		std::cout << "Error compiling shader: " << message.get() << std::endl;
		glDeleteShader(vertex_shader_ID);

		return std::nullopt;
	}

	return vertex_shader_ID;
}

void ShaderProgram::SetNumberOfLights() {
	Bind();
	SetUniform1i("u_dirLights_count", m_dir_lights.size());
	SetUniform1i("u_pointLights_count", m_point_lights.size());
	SetUniform1i("u_flashLights_count", m_flash_lights.size());
}

void ShaderProgram::CreateShaderProgram() {
	GLCall(unsigned int shader_program = glCreateProgram());

	for (const auto& shader : m_shaders) {
		auto compiled_shader = CompileShaderProgram(static_cast<unsigned int>(shader.type), shader.data);
		GLCall(glAttachShader(shader_program, compiled_shader.value()));
		GLCall(glDeleteShader(compiled_shader.value()));
	}

	GLCall(glLinkProgram(shader_program));
	GLCall(glValidateProgram(shader_program));

	m_rendered_ID = shader_program;

	SetNumberOfLights();
}
}
