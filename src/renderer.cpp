#include "epch.h"
#include "renderer.h"
#include "gl_common.h"

namespace engine {
/// Draw array using indices
/// @param t_vao
/// @param t_ibo
/// @param t_shader
void Renderer::Draw(const VertexArray& t_vao, const IndexBuffer& t_ibo, const Shader& t_shader) const {
	t_shader.Bind();
	t_vao.Bind();
	t_ibo.Bind();

	GLCall(glDrawElements(GL_TRIANGLES, t_ibo.GetCount(), GL_UNSIGNED_INT, nullptr));
}

/// Draw whole array without using indices
/// @param t_vao
/// @param t_shader
void Renderer::Draw(const VertexArray& t_vao, const Shader& t_shader) const {
	t_shader.Bind();
	t_vao.Bind();

	GLCall(glDrawArrays(GL_TRIANGLES, 0, t_vao.GetCount()));
}

void Renderer::Clear() const {
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}
}
