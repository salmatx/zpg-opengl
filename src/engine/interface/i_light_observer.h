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

struct FlashLightParams_t {
	float cut_off_angle;
	float outer_cut_off_angle;

	float constant;
	float linear;
	float quadratic;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

struct CameraParams_t {
	glm::vec3 position;
	glm::vec3 direction;
};

class ILightObserver {
public:
	virtual ~ILightObserver() = default;

	virtual void Update(const DirectionalLightParams_t& t_light) = 0;
	virtual void Update(const PointLightParams_t& t_light) = 0;
	virtual void Update(const FlashLightParams_t& t_light, const CameraParams_t& t_camera) = 0;
};
}
