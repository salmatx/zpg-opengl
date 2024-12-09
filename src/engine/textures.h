#pragma once

#include "texture.h"

#include "epch.h"

namespace engine {

class Textures : public IBind {
public:
	Textures(std::initializer_list<std::string> t_paths);
	[[nodiscard]] int GetCount() const {return m_count;}

	void Bind() const;
	void Unbind() const;

private:
	std::array<std::unique_ptr<Texture>, 16> m_textures;
	const int m_count;
};

}
