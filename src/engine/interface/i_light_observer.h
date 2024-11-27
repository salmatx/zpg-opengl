#pragma once

#include "epch.h"

namespace engine {
struct DirectionalLightParams_t {
	glm::vec3 direction;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

class ILightObserver {
public:
	virtual ~ILightObserver() = default;

	virtual void Update(const DirectionalLightParams_t& t_light) = 0;
};
}
