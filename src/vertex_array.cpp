#include "vertex_array.h"

#include "gl_common.h"

namespace engine {
VertexArray::VertexArray() {
	GLCall(glGenVertexArrays(1, &m_renderer_ID));
}

VertexArray::~VertexArray() {
	GLCall(glDeleteVertexArrays(1, &m_renderer_ID));
}

void VertexArray::AddBuffer(const VertexBuffer& t_vb, const VertexBufferLayout& t_layout) {
	this->Bind();
	t_vb.Bind();
	const auto& elements = t_layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); ++i) {
		const auto& element = elements[i];
		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, t_layout.GetStride(), reinterpret_cast<const void*>(offset)));
		offset += element.count * element.size;
	}
}

void VertexArray::Bind() const {
	GLCall(glBindVertexArray(m_renderer_ID));
}

void VertexArray::Unbind() const {
	GLCall(glBindVertexArray(0));
}
}
