#pragma once

#include "texture.h"

#include "epch.h"

namespace engine {

class Textures {
public:
	Textures(std::initializer_list<std::string> t_paths);
	[[nodiscard]] int GetCount() const {return m_count;}

private:
	std::array<std::unique_ptr<Texture>, 16> m_textures;
	const int m_count;
};

}
