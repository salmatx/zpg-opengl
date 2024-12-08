#include "epch.h"
#include "scene.h"

#include "model_rotate.h"
#include "model_scale.h"
#include "model_translate.h"

namespace engine {
void Scene::DrawScene() {
	for (const auto& [key, value] : m_objects) {
		if (value->index_buffer->GetCount() > 0)
			value->drawable_object->Draw(*value->index_buffer);
		else
			value->drawable_object->Draw();
	}
}

void Scene::DrawSkybox() {
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_FALSE);
	for (const auto& [key, value] : m_objects) {
			value->drawable_object->Draw();
	}
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
}

void Scene::ClearScene() {
	for (auto& [key, value] : m_objects) {
		value->drawable_object->Clear();
	}
}

void Scene::AddObjectWithTexture(const std::string& t_name, const std::string& t_path,
	const std::shared_ptr<ShaderProgram>& t_shader) {
	auto mesh = m_model_loader.Load(t_path);
	auto drawable_obj = std::make_unique<DrawableObject>(mesh->vertices.data(), t_shader,
		sizeof(mesh->vertices.data()->params) * mesh->vertices.size(), float{}, 3, float{}, 3, float{}, 2, float{}, 3);
	auto indexes = std::make_unique<IndexBuffer>(mesh->indices.data(), mesh->indices.size());

	AddObject(t_name, std::make_unique<Object_t>(std::move(drawable_obj), std::move(indexes)));
}

void Scene::AddObjectWithoutTexture(const std::string& t_name, std::unique_ptr<DrawableObject> t_drawable_object) {
	auto indexes = std::make_unique<IndexBuffer>(nullptr, 0);
	AddObject(t_name, std::make_unique<Object_t>(std::move(t_drawable_object), std::move(indexes)));
}

void Scene::AddObject(const std::string& t_name, std::unique_ptr<Object_t> t_drawable_object) {
	m_objects.emplace(t_name, std::move(t_drawable_object));
}

void Scene::AddTexture(const std::string& t_object_name, std::initializer_list<std::string> t_paths) {
	m_objects.at(t_object_name)->drawable_object->AddTexture(t_paths);
}

void Scene::AddCubeMap(const std::string& t_object_name, std::initializer_list<std::string> t_paths) {
	m_objects.at(t_object_name)->drawable_object->AddCubeMap(t_paths);
}

bool Scene::RemoveObject(const std::string& t_name) {
	const auto count = std::erase_if(m_objects, [&](const auto& item) {
		const auto& [key, value] = item;
		return key == t_name;
	});

	return count > 0;
}

void Scene::AddTransformation(const std::string& t_name, const Transformation& t_transformation) {
	auto rotate = std::make_unique<engine::ModelRotate>(t_transformation.angle, t_transformation.csys);
	auto translate = std::make_unique<engine::ModelTranslate>(t_transformation.translation);
	auto scale = std::make_unique<engine::ModelScale>(t_transformation.scale);
	auto mode = t_transformation.order;

	switch (mode) {
	case TransformationOrder::TRS:
		m_objects.at(t_name)->drawable_object->AddTransformation({std::move(translate), std::move(rotate), std::move(scale)});
		break;

	case TransformationOrder::RTS:
		m_objects.at(t_name)->drawable_object->AddTransformation({std::move(rotate), std::move(translate), std::move(scale)});
		break;

	default:
		break;
	}
}
}
