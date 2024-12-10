#pragma once

#include "epch.h"

namespace engine {
class DirectionalLight;
class PointLight;
class FlashLight;

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

	bool state;
};

struct CameraParams_t {
	glm::vec3 position;
	glm::vec3 direction;
};

enum class LightType_t {
	Directional,
	Point,
	Flashlight,
};

class ILightObserver {
public:
	virtual ~ILightObserver() = default;

	virtual void Update(const DirectionalLight& t_light) = 0;
	virtual void Update(const PointLight& t_light) = 0;
	virtual void Update(const FlashLight& t_light, const CameraParams_t& t_camera) = 0;
};
}
