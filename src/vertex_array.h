#pragma once
#include "vertex_buffer.h"
#include "vertex_buffer_layout.h"

namespace engine {

class VertexArray : public IBind{
public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& t_vb, const VertexBufferLayout& t_layout);
	void Bind() const;
	void Unbind() const;

private:
	unsigned int m_renderer_ID;
};

}
