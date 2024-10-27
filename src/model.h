#pragma once
#include "vertex_buffer.h"
#include "vertex_buffer_layout.h"

namespace engine {

class Model : public IBind{
public:
	Model(const void* t_vb, const unsigned int t_size);
	~Model();

	void InitModel();
	void Bind() const;
	void Unbind() const;
	[[nodiscard]] inline unsigned int GetCount() const { return m_count; }

	template<typename... Args>
	void SetLayout(Args&&... t_args) {
		static_assert(sizeof...(t_args) % 2 == 0, "Arguments must be provided in pairs (type and count).");

		auto process_layout = [this](auto&& self, auto&& first, auto&& second, auto&&... rest) {
			using T = std::decay_t<decltype(first)>;
			static_assert(std::is_arithmetic_v<T>, "Invalid type.");

			m_layout.Push<T>(second);

			if constexpr (sizeof...(rest) > 0) {
				self(self, std::forward<decltype(rest)>(rest)...);
			}
		};

		process_layout(process_layout, std::forward<Args>(t_args)...);
	}

private:
	unsigned int m_renderer_ID;
	unsigned int m_count = 0;
	VertexBufferLayout m_layout;
	VertexBuffer m_vertex_buffer;
};

}
