#include "vertex_buffer.h"

#include <GL/glew.h>

#include "gl_common.h"

namespace engine {
VertexBuffer::VertexBuffer(const void* t_data, const unsigned int t_size)
	: m_size(t_size) {
	GLCall(glGenBuffers(1, &m_renderer_ID));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_renderer_ID));
	GLCall(glBufferData(GL_ARRAY_BUFFER, t_size, t_data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer() {
	GLCall(glDeleteBuffers(1, &m_renderer_ID));
}

void VertexBuffer::Bind() const {
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_renderer_ID));
}

void VertexBuffer::Unbind() const {
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));

}
}
