#pragma once

#include "epch.h"

namespace engine {

struct Vertex_t {
	float params[11]; // 3*position, 3*normal, 2*texcoord, 3*tangent
};

struct ObjectMesh_t {
	std::vector<Vertex_t> vertices;
	std::vector<unsigned int> indices;
};

class ModelLoader {
public:
	ModelLoader() = default;
	std::shared_ptr<ObjectMesh_t> Load(const std::string& t_path);

};

}
