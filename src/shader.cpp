#include "shader.h"

#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <GL/glew.h>

namespace engine {
Shader::Shader(const std::string& t_filepath)
	: m_filepath(t_filepath) {
	ShaderProgramSource source = this->ParseShader(t_filepath);
	m_rendered_ID = this->CreateShader(source.vertex_source, source.fragment_source);
}

Shader::~Shader() {
	glDeleteProgram(m_rendered_ID);
}

void Shader::Bind() const {
	glUseProgram(m_rendered_ID);
}

void Shader::Unbind() const {
	glUseProgram(0);
}

void Shader::SetUniform4f(const std::string& t_name, float t_v0, float t_v1, float t_v2, float t_v3) {
	const int location = this->GetUniformLocation(t_name);
	glUniform4f(location, t_v0, t_v1, t_v2, t_v3);
}

int Shader::GetUniformLocation(const std::string& t_name) {
	if (m_uniform_location_cache.contains(t_name)) {
		return m_uniform_location_cache.at(t_name);
	}

	const int location = glGetUniformLocation(m_rendered_ID, t_name.c_str());
	if (location == -1) {
		std::cerr << "Failed to get uniform location " << t_name << std::endl;
	}
	m_uniform_location_cache[t_name] = location;

	return location;
}

std::optional<unsigned int> Shader::CompileShader(const unsigned int t_type, const std::string& t_source) {
	unsigned int vertex_shader_ID = glCreateShader(t_type);
	const char* src = t_source.c_str();
	glShaderSource(vertex_shader_ID, 1, &src, nullptr);
	glCompileShader(vertex_shader_ID);

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
	unsigned int shader_program = glCreateProgram();
	auto vertex_shader = this->CompileShader(GL_VERTEX_SHADER, t_vertex_shader);
	auto fragment_shader = this->CompileShader(GL_FRAGMENT_SHADER, t_fragment_shader);

	glAttachShader(shader_program, vertex_shader.value());
	glAttachShader(shader_program, fragment_shader.value());
	glLinkProgram(shader_program);
	glValidateProgram(shader_program);

	glDeleteShader(vertex_shader.value());
	glDeleteShader(fragment_shader.value());

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
