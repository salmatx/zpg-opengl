#include "epch.h"
#include "shader_program.h"
#include "gl_common.h"

namespace engine {
ShaderProgram::ShaderProgram(Camera& t_camera, std::vector<std::shared_ptr<Light>> t_light)
	: m_camera(&t_camera),
	m_loader(ShaderLoader()){
	m_camera->Attach(this);

	m_lights.reserve(t_light.size());
	for(auto& light : t_light) {
		m_lights.emplace_back(std::move(light));
		m_lights.back().get()->Attach(this);
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

void ShaderProgram::SetUniform1f(const std::string& t_name, float t_v0) {
	GLCall(const int location = this->GetUniformLocation(t_name));
	GLCall(glUniform1f(location, t_v0));
}

void ShaderProgram::SetUniform1i(const std::string& t_name, int t_v0) {
	GLCall(const int location = this->GetUniformLocation(t_name));
	GLCall(glUniform1i(location, t_v0));
}

void ShaderProgram::SetUniformMat4f(const std::string& t_name, const glm::mat4& t_matrix) {
	GLCall(glUniformMatrix4fv(this->GetUniformLocation(t_name), 1, GL_FALSE, glm::value_ptr(t_matrix)));
}

void ShaderProgram::LoadShader(const std::string& t_filepath) {
	m_shaders.emplace_back(m_loader.LoadShader(t_filepath));
}

void ShaderProgram::Update(const glm::mat4& t_projection, const glm::mat4& t_view, const glm::vec3& t_position) {
	Bind();
	SetUniformMat4f("u_project_matrix", t_projection);
	SetUniformMat4f("u_view_matrix", t_view);
	SetUniform3f("u_view_position", t_position);
}

void ShaderProgram::Update(const glm::vec3& t_position, const glm::vec3& t_color) {
	Bind();

	int size = m_lights.size();
	SetUniform1i("u_light_count", size);
	for (int i = 0; i < size; ++i) {
		std::string base = "u_lights[" + std::to_string(i) + "]";
		SetUniform3f(base + ".position", m_lights[i]->GetPosition());
		SetUniform3f(base + ".color", m_lights[i]->GetColor());
		SetUniform1f(base + ".intensity", m_lights[i]->GetIntensity());
		SetUniform3f(base + ".direction", m_lights[i]->GetDirection());
		SetUniform1f(base + ".cutoff", glm::cos(glm::radians(m_lights[i]->GetCutoff())));
		SetUniform1f(base + ".outer_cutoff", glm::cos(glm::radians(m_lights[i]->GetOuterCutoff())));
		SetUniform1i(base + ".type", static_cast<int>(m_lights[i]->GetType()));
	}
}

// void ShaderProgram::Update(const std::vector<std::shared_ptr<Light>>& t_lights) {
// 	Bind();
//
// 	int size = t_lights.size();
// 	for (int i = 0; i < size; ++i) {
// 		std::string base = "u_lights[" + std::to_string(i) + "]";
// 		SetUniform3f(base + ".position", t_lights[i]->GetPosition());
// 		SetUniform3f(base + ".color", t_lights[i]->GetColor());
// 		SetUniform1f(base + ".intensity", t_lights[i]->GetIntensity());
// 		SetUniform3f(base + ".direction", t_lights[i]->GetDirection());
// 		SetUniform1f(base + ".cutoff", glm::cos(glm::radians(t_lights[i]->GetCutoff())));
// 		SetUniform1f(base + ".outerCutoff", glm::cos(glm::radians(t_lights[i]->GetOuterCutoff())));
// 		SetUniform1i(base + ".type", static_cast<int>(t_lights[i]->GetType()));
// 	}
// }

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
}
}
