#pragma once

#include <i_bind.h>

#include "epch.h"

namespace engine {

class Cubemap : public IBind {
public:
	Cubemap(std::initializer_list<std::string> t_paths);
	[[nodiscard]] int GetCount() const {return 1;}

	void Bind() const;
	void Unbind() const;

private:
	void Load(std::initializer_list<std::string> t_paths);

	unsigned int m_renderer_ID {};
};

}
