#pragma once
#include "model_transformation.h"

namespace engine {

class ModelTranslate : public ModelTransformation {
public:
	ModelTranslate(const glm::vec3& t_translation);
	~ModelTranslate() override = default;

	[[nodiscard]] glm::mat4 Transform(const glm::mat4& model) const override;

private:
	glm::vec3 m_translation;
};

}
