#include "textures.h"

namespace engine {
Textures::Textures(std::initializer_list<std::string> t_paths)
	: m_count(t_paths.size()) {
	int count = 0;
	for (const auto& path : t_paths) {
		m_textures[count] = std::make_unique<Texture>(path, count);
		count++;
	}
}

void Textures::Bind() const {
	for (int i = 0; i < m_count; i++) {
		m_textures[i]->Bind();
	}
}

void Textures::Unbind() const {
	for (int i = 0; i < m_count; i++) {
		m_textures[i]->Unbind();
	}
}
}
