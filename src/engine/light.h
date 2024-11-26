#pragma once

#include <i_light_subject.h>

#include "epch.h"

namespace engine {

struct LightParams {
	glm::vec3 position;
	glm::vec3 color;
};

class Light : public ILightSubject {
public:
	Light(const LightParams& t_params);

	void SetPosition(const glm::vec3& t_position);
	void SetColor(const glm::vec3& t_color);
	void InitLight();

	void Attach(ILightObserver* observer) override;
	void Detach(ILightObserver* observer) override;

private:
	void Notify() override;

	std::list<ILightObserver*> m_observers;
	glm::vec3 m_position;
	glm::vec3 m_color;
};
}

