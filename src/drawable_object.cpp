#include "epch.h"
#include "drawable_object.h"
#include "gl_common.h"
#include "model_translate.h"

namespace engine {
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

/// Draw array using indices
/// @param t_ibo
/// @param t_shader
void DrawableObject::Draw(const IndexBuffer& t_ibo, const Shader& t_shader) const {
	auto shader = t_shader;
	shader.Bind();
	m_model.Bind();
	t_ibo.Bind();

	glm::mat4 model = glm::mat4(1.0f);
	int count = 0;
	for (const auto& transformation : m_transformations) {
		count++;
		model = transformation->Transform(model);
		shader.SetUniformMat4f("u_model_matrix", model);
		if (count == 3) {
			GLCall(glDrawArrays(GL_TRIANGLES, 0, m_model.GetCount()));
			model = glm::mat4(1.0f);
			count = 0;
		}
	}
}

/// Draw whole array without using indices
/// @param t_shader
void DrawableObject::Draw(const Shader& t_shader) const {
	auto shader = t_shader;
	shader.Bind();
	m_model.Bind();

	glm::mat4 model = glm::mat4(1.0f);
	int count = 0;
	for (const auto& transformation : m_transformations) {
		count++;
		model = transformation->Transform(model);
		shader.SetUniformMat4f("u_model_matrix", model);
		if (count == 3) {
			GLCall(glDrawArrays(GL_TRIANGLES, 0, m_model.GetCount()));
			model = glm::mat4(1.0f);
			count = 0;
		}
	}
}


void DrawableObject::Clear() {
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

	m_transformations.clear();
}
}
