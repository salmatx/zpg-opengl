#include "cubemap.h"

#include <gl_common.h>

namespace engine {
Cubemap::Cubemap(std::initializer_list<std::string> t_paths) {
	GLCall(glActiveTexture(GL_TEXTURE0));
	Load(t_paths);
	Cubemap::Bind();
}

void Cubemap::Bind() const {
	GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_renderer_ID));
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
}

void Cubemap::Unbind() const {
	GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
}

void Cubemap::Load(std::initializer_list<std::string> t_paths) {
	auto count = t_paths.size();
	if (count != 6) {
		std::cerr << "Invalid number of textures for cubemap, actual count: " << count << std::endl;
		return;
	}

	auto jpg_count = 0;
	auto png_count = 0;

	for (const auto& t_path : t_paths) {
		auto extensionPos = t_path.find_last_of(".");
		if (extensionPos == std::string::npos) {
			std::cerr << "Invalid texture file: No extension found (" << t_path << ")" << std::endl;
			return;
		}

		std::string extension = t_path.substr(extensionPos + 1);
		std::ranges::transform(extension, extension.begin(), ::tolower);
		if (extension == "png")
			++png_count;
		else if (extension == "jpg" || extension == "jpeg")
			++jpg_count;
	}

	if (!(jpg_count == 6 || png_count == 6)) {
		std::cerr << "Error: Format of all textures must match, actual count (jpg, png) = ("
		<< jpg_count << ", " << png_count << ")" << std::endl;
		return;
	}

	auto extensionPos = t_paths.begin()->find_last_of(".");
	std::string extension = t_paths.begin()->substr(extensionPos + 1);

	std::vector paths = t_paths;

	if (extension == "png") {
		m_renderer_ID = SOIL_load_OGL_cubemap(paths.at(0).c_str(), paths.at(1).c_str(),
			paths.at(2).c_str(), paths.at(3).c_str(),
			paths.at(4).c_str(), paths.at(5).c_str(),
			SOIL_LOAD_RGBA, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	}
	else if (extension == "jpg" || extension == "jpeg") {
		m_renderer_ID = SOIL_load_OGL_cubemap(paths.at(0).c_str(), paths.at(1).c_str(),
			paths.at(2).c_str(), paths.at(3).c_str(),
			paths.at(4).c_str(), paths.at(5).c_str(),
			SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	}

	if (m_renderer_ID == NULL) {
		std::cerr << "Failed to load textures: " << SOIL_last_result() << std::endl;
	}
}
}
