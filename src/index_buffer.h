#pragma once
#include "i_bind.h"

namespace engine {

class IndexBuffer : public IBind{
public:
	IndexBuffer(const unsigned int* t_data, const unsigned int t_count);
	~IndexBuffer();

	void Bind() const override;
	void Unbind() const override;
	[[nodiscard]] inline unsigned int GetCount() const { return m_count; }

private:
	unsigned int m_renderer_ID;
	unsigned int m_count;
};

}
