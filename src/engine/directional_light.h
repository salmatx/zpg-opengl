#pragma once

#include <light.h>

#include "epch.h"

namespace engine {

struct DirectionalLightParams_t {
	glm::vec3 position;
	glm::vec3 color;
};

class DirectionalLight : public Light {
public:
	explicit DirectionalLight(const DirectionalLightParams_t& t_params);
};
}

