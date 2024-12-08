#pragma once
#include "drawable_object.h"

namespace engine {

enum class TransformationOrder {
	TRS,
	RTS
};

struct Transformation {
	glm::vec3 translation;
	glm::vec3 csys;
	glm::vec3 scale;
	float angle;
	TransformationOrder order;
};

class Scene {
public:
	Scene() = default;
	~Scene() = default;

	void DrawScene();
	void ClearScene();
	void AddObject(const std::string& t_name, std::unique_ptr<DrawableObject> t_drawable_object);
	void AddTexture(const std::string& t_object_name, std::initializer_list<std::string> t_paths);
	bool RemoveObject(const std::string& t_name);
	void AddTransformation(const std::string& t_name, const Transformation& t_transformation);

private:
	std::unordered_map<std::string, std::unique_ptr<DrawableObject>> m_objects;
};

}
