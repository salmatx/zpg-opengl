#pragma once

#include "epch.h"

namespace engine {
class ModelTransformation {
public:
	virtual ~ModelTransformation() = default;
	[[nodiscard]] virtual glm::mat4 Transform(const glm::mat4& t_model) const = 0;
	[[nodiscard]] virtual std::unique_ptr<ModelTransformation> Clone() const = 0;
};

}
