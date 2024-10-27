#include "model_scale.h"

namespace engine {
ModelScale::ModelScale(const glm::vec3& t_scale)
	: m_scale(t_scale) {}

glm::mat4 ModelScale::Transform(const glm::mat4& t_model) const {
	return glm::scale(t_model, m_scale);
}
}
