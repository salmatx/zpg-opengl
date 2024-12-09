#pragma once

#include "epch.h"
#include "i_bind.h"

namespace engine {

class Texture : public IBind {
public:
	Texture(const std::string& t_path, int t_index);

	void Bind() const;
	void Unbind() const;

private:
	void Load(const std::string& t_path);

	unsigned int m_renderer_ID {};
	int m_index {};
};

}
