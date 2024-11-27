#pragma once

#include "epch.h"

namespace engine {
struct DirectionalLightParams_t {
	glm::vec3 direction;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

struct PointLightParams_t {
	glm::vec3 position;

	float constant;
	float linear;
	float quadratic;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

class ILightObserver {
public:
	virtual ~ILightObserver() = default;

	virtual void Update(const DirectionalLightParams_t& t_light) = 0;
	virtual void Update(const PointLightParams_t& t_light) = 0;
};
}
