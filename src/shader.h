#pragma once
#include <optional>
#include <string>
#include <unordered_map>

#include "i_bind.h"

namespace engine {

struct ShaderProgramSource {
	std::string vertex_source;
	std::string fragment_source;
};

class Shader : public IBind {
public:
	explicit Shader(const std::string& t_filepath);
	~Shader();

	void Bind() const;
	void Unbind() const;
	void SetUniform4f(const std::string& t_name, float t_v0, float t_v1, float t_v2, float t_v3);

private:
	int GetUniformLocation(const std::string& t_name);
	ShaderProgramSource ParseShader(const std::string& t_filepath);
	std::optional<unsigned int> CompileShader(unsigned int t_type, const std::string& t_source);
	unsigned int CreateShader(const std::string& t_vertex_shader, const std::string& t_fragment_shader);

	unsigned int m_rendered_ID {0};
	std::string m_filepath;
	std::unordered_map<std::string, int> m_uniform_location_cache;
};

}
