#pragma once
#include "model_transformation.h"

namespace engine {

class ModelRotate : public ModelTransformation {
public:
	ModelRotate(float t_angle, const glm::vec3& t_csys);
	~ModelRotate() override = default;

	[[nodiscard]] glm::mat4 Transform(const glm::mat4& t_model) const override;

private:
	float m_angle;
	glm::vec3 m_csys;
};

}
