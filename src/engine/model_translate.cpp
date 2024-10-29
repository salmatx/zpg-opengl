#include "model_translate.h"

namespace engine {
ModelTranslate::ModelTranslate(const glm::vec3& t_translation)
	: m_translation(t_translation) {}

glm::mat4 ModelTranslate::Transform(const glm::mat4& model) const {
	return glm::translate(model, m_translation);
}

std::unique_ptr<ModelTransformation> ModelTranslate::Clone() const {
	return std::make_unique<ModelTranslate>(*this);
}
}
