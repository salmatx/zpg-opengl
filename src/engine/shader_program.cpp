#include "epch.h"
#include "shader_program.h"
#include "gl_common.h"

namespace engine {
ShaderProgram::ShaderProgram(const std::shared_ptr<Camera>& t_camera, const std::vector<std::shared_ptr<Light>>& t_lights)
	: m_camera(t_camera),
	m_lights(t_lights),
	m_loader(ShaderLoader()){
	m_camera->Attach(this);
	for(auto& light : m_lights) {
		light->Attach(this);
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
	SetUniform3f("u_light_position", t_position);
	SetUniform3f("u_light_color", t_color);
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
