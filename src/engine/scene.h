#pragma once
#include <model_loader.h>

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

struct Object_t {
	std::unique_ptr<DrawableObject> drawable_object;
	std::unique_ptr<IndexBuffer> index_buffer;
};

class Scene {
public:
	Scene() = default;
	~Scene() = default;

	void DrawScene();
	void DrawSkybox();
	void ClearScene();
	void AddObjectWithTexture(const std::string& t_name, const std::string& t_path, const std::shared_ptr<ShaderProgram>& t_shader);
	void AddObjectWithoutTexture(const std::string& t_name, std::unique_ptr<DrawableObject> t_drawable_object);
	void AddTexture(const std::string& t_object_name, std::initializer_list<std::string> t_paths);
	void AddCubeMap(const std::string& t_object_name, std::initializer_list<std::string> t_paths);
	bool RemoveObject(const std::string& t_name);
	void AddTransformation(const std::string& t_name, const Transformation& t_transformation);

private:
	void AddObject(const std::string& t_name, std::unique_ptr<Object_t> t_drawable_object);

	std::unordered_map<std::string, std::unique_ptr<Object_t>> m_objects;
	ModelLoader m_model_loader {};
};

}
