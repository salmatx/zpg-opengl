#pragma once

#include "epch.h"

namespace engine {

class ICameraObserver {
public:
	virtual ~ICameraObserver() = default;

	virtual void Update(const glm::mat4& t_projection, const glm::mat4& t_view) = 0;
};
}