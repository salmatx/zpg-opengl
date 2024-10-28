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
	explicit Scene(std::shared_ptr<Camera> t_camera);
	~Scene() = default;

	void DrawScene();
	void ClearScene();
	void AddObject(const std::string& t_name, const DrawableObject& t_drawable_object);
	bool RemoveObject(const std::string& t_name);
	void AddTransformation(const std::string& t_name, const Transformation& t_transformation);
	void AddShader(const std::string& t_name, const Shader& t_shader);
	bool RemoveShader(const std::string& t_name);
	void SelectShader(const std::string& t_name);

private:
	std::unordered_map<std::string, DrawableObject> m_objects;
	std::unordered_map<std::string, Shader> m_shaders;
	std::shared_ptr<Camera> m_camera;
	Shader* m_current_shader;
};

}
