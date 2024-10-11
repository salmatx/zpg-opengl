#pragma once
#include "i_bind.h"

namespace engine {

class VertexBuffer : public IBind {
public:
    VertexBuffer(const void* t_data, const unsigned int t_size);
    ~VertexBuffer();

    void Bind() const override;
    void Unbind() const override;

private:
    unsigned int m_renderer_ID;
};

}
