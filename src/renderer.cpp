#include "renderer.h"

namespace engine {
void Renderer::Draw(const VertexArray& t_vao, const IndexBuffer& t_ibo, const Shader& t_shader) const {
	t_shader.Bind();
	t_vao.Bind();
	t_ibo.Bind();

	glDrawElements(GL_TRIANGLES, t_ibo.GetCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::Clear() const {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
}
