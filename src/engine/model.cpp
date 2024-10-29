#include "epch.h"
#include "model.h"
#include "gl_common.h"

namespace engine {
Model::Model(const void* t_vb, const unsigned int t_size)
	: m_vertex_buffer(std::make_shared<VertexBuffer>(t_vb, t_size)) {
	GLCall(glGenVertexArrays(1, &m_renderer_ID));
}

Model::~Model() {
	GLCall(glDeleteVertexArrays(1, &m_renderer_ID));
}

void Model::InitModel() {
	this->Bind();
	m_vertex_buffer->Bind();
	const auto& elements = m_layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); ++i) {
		const auto& element = elements[i];
		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, m_layout.GetStride(), reinterpret_cast<const void*>(offset)));
		offset += element.count * element.size;
	}
	m_count = m_vertex_buffer->GetSize() / offset;
}

void Model::Bind() const {
	GLCall(glBindVertexArray(m_renderer_ID));
}

void Model::Unbind() const {
	GLCall(glBindVertexArray(0));
}
}
