#include "texture.h"

#include <gl_common.h>
#include <algorithm>

namespace engine {
Texture::Texture(const std::string& t_path, int t_index)
	: m_index(t_index) {
	GLCall(glActiveTexture(GL_TEXTURE0 + m_index));
	Load(t_path);
	Texture::Bind();
}

void Texture::Bind() const {
	GLCall(glBindTexture(GL_TEXTURE_2D, m_renderer_ID));
}

void Texture::Unbind() const {
	GLCall(glActiveTexture(GL_TEXTURE0 + m_index));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture::Load(const std::string& t_path) {
	auto extensionPos = t_path.find_last_of(".");
	if (extensionPos == std::string::npos) {
		std::cerr << "Invalid texture file: No extension found (" << t_path << ")" << std::endl;
		return;
	}

	std::string extension = t_path.substr(extensionPos + 1);
	std::ranges::transform(extension, extension.begin(), ::tolower);
	if (extension == "png") {
		m_renderer_ID = SOIL_load_OGL_texture(t_path.c_str(),
			SOIL_LOAD_RGBA, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y | SOIL_FLAG_MIPMAPS);
	}
	else if (extension == "jpg" || extension == "jpeg") {
		m_renderer_ID = SOIL_load_OGL_texture(t_path.c_str(),
			SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y | SOIL_FLAG_MIPMAPS);
	}

	if (m_renderer_ID == NULL) {
		std::cerr << "Failed to load texture: (" << t_path << "): " << SOIL_last_result() << std::endl;
	}
}
}
