#pragma once
#include "model_transformation.h"

namespace engine {

class ModelScale : public ModelTransformation {
public:
	ModelScale(const glm::vec3& t_scale);
	~ModelScale() override = default;

	[[nodiscard]] glm::mat4 Transform(const glm::mat4& t_model) const override;

private:
	glm::vec3 m_scale;
};

}
