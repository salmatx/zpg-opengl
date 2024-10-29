#include "epch.h"
#include "scene.h"

#include "model_rotate.h"
#include "model_scale.h"
#include "model_translate.h"

namespace engine {
Scene::Scene(Camera& t_camera)
	: m_camera(&t_camera) {}

void Scene::DrawScene() {
	for (const auto& [key, value] : m_objects) {
		value->Draw(m_current_shader);
	}
}

void Scene::ClearScene() {
	for (auto& [key, value] : m_objects) {
		value->Clear();
	}
}

void Scene::AddObject(const std::string& t_name, const DrawableObject& t_drawable_object) {
	m_objects.emplace(t_name, std::make_unique<DrawableObject>(t_drawable_object));
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
		m_objects.at(t_name)->AddTransformation({std::move(translate), std::move(rotate), std::move(scale)});
		break;

	case TransformationOrder::RTS:
		m_objects.at(t_name)->AddTransformation({std::move(rotate), std::move(translate), std::move(scale)});
		break;

	default:
		break;
	}
}

void Scene::AddShader(const std::string& t_name, std::string t_path) {
	m_shaders.emplace(t_name, std::make_shared<Shader>(*m_camera, t_path));
}

bool Scene::RemoveShader(const std::string& t_name) {
	const auto count = std::erase_if(m_shaders, [&](const auto& item) {
		const auto& [key, value] = item;
		return key == t_name;
	});

	return count > 0;
}

void Scene::SelectShader(const std::string& t_name) {
	m_current_shader = m_shaders.at(t_name);
}
}
