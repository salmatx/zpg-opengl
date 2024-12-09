#include "epch.h"
#include "drawable_object.h"

#include <cubemap.h>

#include "gl_common.h"
#include "model_translate.h"

namespace engine {
DrawableObject::DrawableObject(const DrawableObject& t_other)
	: m_model(t_other.m_model) {
	m_transformations.reserve(t_other.m_transformations.size());
	for (const auto& transformation : t_other.m_transformations) {
		m_transformations.emplace_back(transformation->Clone());
	}
}

DrawableObject& DrawableObject::operator=(const DrawableObject& t_other) {
	DrawableObject tmp(t_other);
	std::swap(m_model, tmp.m_model);
	std::swap(m_transformations, tmp.m_transformations);
	std::swap(m_shader, tmp.m_shader);

	return *this;
}

void DrawableObject::AddTransformation(std::unique_ptr<ModelTransformation> (& t_transformation)[3]) {
	int size = 3;
	for (unsigned int i = 0; i < size; ++i) {
		m_transformations.emplace_back(std::move(t_transformation[i]));
	}
}

void DrawableObject::AddTransformation(std::unique_ptr<ModelTransformation>(&& t_transformation)[3]) {
	int size = 3;
	for (unsigned int i = 0; i < size; ++i) {
		m_transformations.emplace_back(std::move(t_transformation[i]));
	}
}

void DrawableObject::AddTexture(std::initializer_list<std::string> t_paths) {
	m_textures = std::make_unique<Textures>(t_paths);
}

void DrawableObject::AddCubeMap(std::initializer_list<std::string> t_paths) {
	m_cubemap = std::make_unique<Cubemap>(t_paths);
}

/// Draw array using indices
/// @param t_ibo
void DrawableObject::Draw(const IndexBuffer& t_ibo) const {
	m_shader->Bind();
	m_model->Bind();
	t_ibo.Bind();

	auto text_count = 0;
	if (m_textures != nullptr) {
		text_count = m_textures->GetCount();
		m_textures->Bind();
	}
	else if (m_cubemap != nullptr) {
		text_count = m_cubemap->GetCount();
		m_cubemap->Bind();
	}
	for (int i = 0; i < text_count; ++i) {
		m_shader->SetTexture(i);
	}

	glm::mat4 model = glm::mat4(1.0f);
	int count = 0;
	for (const auto& transformation : m_transformations) {
		count++;
		model = transformation->Transform(model);
		m_shader->SetTransformation(model);
		if (count == 3) {
			GLCall(glDrawElements(GL_TRIANGLES, t_ibo.GetCount(), GL_UNSIGNED_INT, nullptr));
			model = glm::mat4(1.0f);
			count = 0;
		}
	}
}

/// Draw whole array without using indices
void DrawableObject::Draw() const {
	m_shader->Bind();
	m_model->Bind();

	auto text_count = 0;
	if (m_textures != nullptr) {
		text_count = m_textures->GetCount();
		m_textures->Bind();
	}
	else if (m_cubemap != nullptr) {
		text_count = m_cubemap->GetCount();
		m_cubemap->Bind();
	}
	for (int i = 0; i < text_count; ++i) {
		m_shader->SetTexture(i);
	}

	glm::mat4 model = glm::mat4(1.0f);
	int count = 0;
	for (const auto& transformation : m_transformations) {
		count++;
		model = transformation->Transform(model);
		m_shader->SetTransformation(model);
		if (count == 3) {
			GLCall(glDrawArrays(GL_TRIANGLES, 0, m_model->GetCount()));
			model = glm::mat4(1.0f);
			count = 0;
		}
	}
}

void DrawableObject::Clear() {
	m_transformations.clear();
}
}
