#pragma once

#include "index_buffer.h"
#include "shader.h"
#include "model.h"
#include "model_transformation.h"

namespace engine {

class DrawableObject {
public:
	template<typename... Args>
	DrawableObject(const void* t_vb, const unsigned int t_size, Args&&... t_args)
		: m_model(t_vb, t_size) {
		m_model.SetLayout(t_args...);
		m_model.InitModel();
	}

	~DrawableObject() = default;

	void AddTransformation(std::unique_ptr<ModelTransformation> (& t_transformation)[3]);
	void AddTransformation(std::unique_ptr<ModelTransformation> (&& t_transformation)[3]);
	void Draw(const IndexBuffer& t_ibo, const Shader& t_shader) const;
	void Draw(const Shader& t_shader) const;
	void Clear();

private:
    Model m_model;
	std::vector<std::unique_ptr<ModelTransformation>> m_transformations;
};

}
