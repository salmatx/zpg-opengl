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
}
