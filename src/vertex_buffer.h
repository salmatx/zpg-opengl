#pragma once

#include "i_bind.h"

namespace engine {

class VertexBuffer : public IBind {
public:
    VertexBuffer(const void* t_data, const unsigned int t_size);
    ~VertexBuffer();

    void Bind() const override;
    void Unbind() const override;
    [[nodiscard]] inline unsigned int GetSize() const { return m_size; }

private:
    unsigned int m_renderer_ID;
    unsigned int m_size;
};

}
