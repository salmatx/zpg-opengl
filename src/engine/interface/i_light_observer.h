#pragma once

#include "epch.h"

namespace engine {

class ILightObserver {
public:
	virtual ~ILightObserver() = default;

	virtual void Update(const glm::vec3& t_position, const glm::vec3& t_color) = 0;
};
}
