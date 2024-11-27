#pragma once

#include "i_light_observer.h"

#include "epch.h"

namespace engine {

struct LightParams_t {
	glm::vec3 position;
	glm::vec3 color;
};

class Light {
public:
	explicit Light(const LightParams_t& t_params);
	virtual ~Light() = default;

	virtual void Notify();

	void Attach(ILightObserver* t_observer);
	void Detach(ILightObserver* t_observer);
	void InitLight();
	void SetPosition(const glm::vec3& t_position);
	void SetColor(const glm::vec3& t_color);

private:
	std::list<ILightObserver*> m_observers;
	glm::vec3 m_position;
	glm::vec3 m_color;
};
}