#include "vertex_buffer_layout.h"

namespace engine {
VertexBufferLayout::VertexBufferLayout()
	: m_stride(0) {}

template <>
void VertexBufferLayout::Push<float>(int t_count) {
	auto size = sizeof(GLfloat);

	m_elements.emplace_back(size, GL_FLOAT, t_count, GL_FALSE);
	m_stride += t_count * size;
}

template <>
void VertexBufferLayout::Push<unsigned int>(int t_count) {
	auto size = sizeof(GLuint);

	m_elements.emplace_back(size, GL_UNSIGNED_INT, t_count, GL_FALSE);
	m_stride += t_count * size;
}

template <>
void VertexBufferLayout::Push<unsigned char>(int t_count) {
	auto size = sizeof(GLbyte);

	m_elements.emplace_back(size, GL_UNSIGNED_BYTE, t_count, GL_TRUE);
	m_stride += t_count * size;
}
}
