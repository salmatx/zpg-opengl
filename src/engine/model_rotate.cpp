#include "model_rotate.h"

namespace engine {
ModelRotate::ModelRotate(float t_angle, const glm::vec3& t_csys)
	: m_angle(t_angle), m_csys(t_csys) {}

glm::mat4 ModelRotate::Transform(const glm::mat4& t_model) const {
	return glm::rotate(t_model, glm::radians(m_angle), m_csys);
}

std::unique_ptr<ModelTransformation> ModelRotate::Clone() const {
	return std::make_unique<ModelRotate>(*this);
}
}
