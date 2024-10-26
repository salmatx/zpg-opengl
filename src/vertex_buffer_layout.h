#pragma once

namespace engine {

template <typename T>
concept AllowedType = std::same_as<T, float>
	|| std::same_as<T, unsigned int>
	|| std::same_as<T, unsigned char>;

struct VertexBufferElement {
	size_t size;
	unsigned int type;
	int count;
	unsigned char normalized;
};

class VertexBufferLayout {
public:
	VertexBufferLayout();
	~VertexBufferLayout() = default;

	template<typename T>
	void Push(int t_count) {
		static_assert(AllowedType<T>, "Invalid type passed to Push");
	}

	inline const std::vector<VertexBufferElement>& GetElements() const { return m_elements; }
	inline int GetStride() const { return m_stride; }
private:
	std::vector<VertexBufferElement> m_elements;
	unsigned int m_stride;
};

}
