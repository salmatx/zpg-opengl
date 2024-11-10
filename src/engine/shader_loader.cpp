#include "shader_loader.h"

namespace engine {
Shader ShaderLoader::LoadShader(const std::string& t_filepath) {
	std::ifstream stream(t_filepath);

	if (!stream.is_open()) {
		std::cerr << "Failed to open file \"" << t_filepath << "\"" << std::endl;
		return {};
	}

	std::string line;
	std::stringstream ss;
	ShaderType type = ShaderType::none;
	while (std::getline(stream, line)) {
		if (line.find("#shader") != std::string::npos) {
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::vertex;
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::fragment;
		}
		else {
			ss << line << std::endl;
		}
	}

	return {type, ss.str()};
}
}
