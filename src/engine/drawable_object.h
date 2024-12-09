#pragma once

#include <cubemap.h>
#include <textures.h>

#include "index_buffer.h"
#include "shader_program.h"
#include "model.h"
#include "model_transformation.h"

namespace engine {

class DrawableObject {
public:
	template<typename... Args>
	DrawableObject(const void* t_vb, std::shared_ptr<ShaderProgram> t_shader, const unsigned int t_size, Args&&... t_args)
		: m_model(std::make_shared<Model>(t_vb, t_size)),
		m_shader(std::move(t_shader)) {
		m_model->SetLayout(t_args...);
		m_model->InitModel();
	}

	DrawableObject(const DrawableObject& t_other);
	DrawableObject& operator=(const DrawableObject& t_other);

	~DrawableObject() = default;

	void AddTransformation(std::unique_ptr<ModelTransformation> (& t_transformation)[3]);
	void AddTransformation(std::unique_ptr<ModelTransformation> (&& t_transformation)[3]);
	void AddTexture(std::initializer_list<std::string> t_paths);
	void AddCubeMap(std::initializer_list<std::string> t_paths);
	void Draw(const IndexBuffer& t_ibo) const;
	void Draw() const;
	void Clear();

private:
    std::shared_ptr<Model> m_model;
	std::vector<std::unique_ptr<ModelTransformation>> m_transformations;
	std::shared_ptr<ShaderProgram> m_shader;
	std::unique_ptr<Textures> m_textures = nullptr;
	std::unique_ptr<Cubemap> m_cubemap = nullptr;
};

}
