#include "index_buffer.h"

#include <GL/glew.h>

namespace engine {
IndexBuffer::IndexBuffer(const unsigned int* t_data, const unsigned int t_count)
	: m_count(t_count) {
	glGenBuffers(1, &m_renderer_ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer_ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(*t_data) * t_count, t_data, GL_DYNAMIC_DRAW);
	GLsizeiptr num_indices;
}

IndexBuffer::~IndexBuffer() {
	glDeleteBuffers(1, &m_renderer_ID);
}

void IndexBuffer::Bind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer_ID);

}

void IndexBuffer::Unbind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}
}
