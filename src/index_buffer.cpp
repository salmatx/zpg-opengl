#include "epch.h"
#include "index_buffer.h"

#include "gl_common.h"

namespace engine {
IndexBuffer::IndexBuffer(const unsigned int* t_data, const unsigned int t_count)
	: m_count(t_count) {
	GLCall(glGenBuffers(1, &m_renderer_ID));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer_ID));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(*t_data) * t_count, t_data, GL_DYNAMIC_DRAW));
}

IndexBuffer::~IndexBuffer() {
	GLCall(glDeleteBuffers(1, &m_renderer_ID));
}

void IndexBuffer::Bind() const {
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer_ID));

}

void IndexBuffer::Unbind() const {
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

}
}
