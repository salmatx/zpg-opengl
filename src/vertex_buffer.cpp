#include "vertex_buffer.h"

#include <GL/glew.h>

namespace engine {
VertexBuffer::VertexBuffer(const void* t_data, const unsigned int t_size) {
	glGenBuffers(1, &m_renderer_ID);
	glBindBuffer(GL_ARRAY_BUFFER, m_renderer_ID);
	glBufferData(GL_ARRAY_BUFFER, t_size, t_data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer() {
	glDeleteBuffers(1, &m_renderer_ID);
}

void VertexBuffer::Bind() const {
	glBindBuffer(GL_ARRAY_BUFFER, m_renderer_ID);

}

void VertexBuffer::Unbind() const {
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}
}
