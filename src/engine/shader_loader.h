#pragma once

#include "epch.h"

namespace engine {

enum class ShaderType {
	none = -1, vertex = GL_VERTEX_SHADER, fragment = GL_FRAGMENT_SHADER, light = 2
};

struct Shader {
	ShaderType type;
	std::string data;
};

class ShaderLoader {
public:
	ShaderLoader() = default;
	~ShaderLoader() = default;
	Shader LoadShader(const std::string& t_filepath);
};

}
