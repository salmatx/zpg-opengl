#pragma once

#include "epch.h"

namespace engine {
class Light;

class ILightObserver {
public:
	virtual ~ILightObserver() = default;

	virtual void Update(const glm::vec3& t_position, const glm::vec3& t_color) = 0;
	// virtual void Update(const std::vector<std::shared_ptr<Light>>& t_lights) = 0;
};
}
