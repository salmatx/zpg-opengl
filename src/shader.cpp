#include "epch.h"
#include "shader.h"
#include "gl_common.h"

namespace engine {
Shader::Shader(Camera& t_camera, const std::string& t_filepath)
	: m_camera(t_camera),
	m_filepath(t_filepath) {
	ShaderProgramSource source = this->ParseShader(t_filepath);
	m_rendered_ID = this->CreateShader(source.vertex_source, source.fragment_source);

	m_camera.Attach(this);
}

Shader::Shader(const Shader& t_other)
	: m_rendered_ID(t_other.m_rendered_ID),
	m_camera(t_other.m_camera),
	m_filepath(t_other.m_filepath),
	m_uniform_location_cache(t_other.m_uniform_location_cache) {
	ShaderProgramSource source = this->ParseShader(m_filepath);

	m_camera.Attach(this);
}

Shader& Shader::operator=(const Shader& t_other) {
	Shader tmp(t_other);
	std::swap(m_filepath, tmp.m_filepath);
	std::swap(m_rendered_ID, tmp.m_rendered_ID);
	std::swap(m_uniform_location_cache, tmp.m_uniform_location_cache);

	return *this;
}

Shader::~Shader() {
	GLCall(glDeleteProgram(m_rendered_ID));
	m_camera.Detach(this);
}

void Shader::Bind() const {
	GLCall(glUseProgram(m_rendered_ID));
}

void Shader::Unbind() const {
	GLCall(glUseProgram(0));
}

void Shader::SetUniform4f(const std::string& t_name, float t_v0, float t_v1, float t_v2, float t_v3) {
	GLCall(const int location = this->GetUniformLocation(t_name));
	GLCall(glUniform4f(location, t_v0, t_v1, t_v2, t_v3));
}

void Shader::SetUniformMat4f(const std::string& t_name, const glm::mat4& t_matrix) {
	GLCall(glUniformMatrix4fv(this->GetUniformLocation(t_name), 1, GL_FALSE, glm::value_ptr(t_matrix)));
}

void Shader::Update(const glm::mat4& t_projection, const glm::mat4& t_view) {
	SetUniformMat4f("u_project_matrix", t_projection);
	SetUniformMat4f("u_view_matrix", t_view);
}

void Shader::RemoveObservation() {
	m_camera.Detach(this);
}

int Shader::GetUniformLocation(const std::string& t_name) {
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

std::optional<unsigned int> Shader::CompileShader(const unsigned int t_type, const std::string& t_source) {
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

unsigned int Shader::CreateShader(const std::string& t_vertex_shader, const std::string& t_fragment_shader) {
	GLCall(unsigned int shader_program = glCreateProgram());
	auto vertex_shader = this->CompileShader(GL_VERTEX_SHADER, t_vertex_shader);
	auto fragment_shader = this->CompileShader(GL_FRAGMENT_SHADER, t_fragment_shader);

	GLCall(glAttachShader(shader_program, vertex_shader.value()));
	GLCall(glAttachShader(shader_program, fragment_shader.value()));
	GLCall(glLinkProgram(shader_program));
	GLCall(glValidateProgram(shader_program));

	GLCall(glDeleteShader(vertex_shader.value()));
	GLCall(glDeleteShader(fragment_shader.value()));

	return shader_program;
}


ShaderProgramSource Shader::ParseShader(const std::string& t_filepath) {
	std::ifstream stream(t_filepath);

	if (!stream.is_open()) {
		std::cerr << "Failed to open file \"" << t_filepath << "\"" << std::endl;
		return {};
	}

	enum class ShaderType {
		none = -1, vertex = 0, fragment = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::none;
	while (std::getline(stream, line)) {
		if (line.find("#shader") != std::string::npos) {
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::vertex;
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::fragment;
		}
		else {
			ss[static_cast<int>(type)] << line << std::endl;
		}
	}

	return { ss[0].str(), ss[1].str() };
}

}
