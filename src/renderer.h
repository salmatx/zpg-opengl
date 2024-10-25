#pragma once

#include "index_buffer.h"
#include "shader.h"
#include "vertex_array.h"

namespace engine {

class Renderer {
public:
	Renderer() = default;
	~Renderer() = default;

	void Draw(const VertexArray& t_vao, const IndexBuffer& t_ibo, const Shader& t_shader) const;
	void Draw(const VertexArray& t_vao, const Shader& t_shader) const;
	void Clear() const;
};

}
