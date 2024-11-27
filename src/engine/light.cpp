#include "light.h"

namespace engine {

Light::Light(const LightParams_t& t_params)
	: m_position(t_params.position), m_color(t_params.color) {}

void Light::Attach(ILightObserver* t_observer) {
	m_observers.emplace_back(t_observer);
}
void Light::Detach(ILightObserver* t_observer) {
	m_observers.remove(t_observer);
}
void Light::InitLight() {
	Notify();
}
void Light::Notify() {
	for (ILightObserver* observer : m_observers) {
		observer->Update(m_position, m_color);
	}
}

void Light::SetPosition(const glm::vec3& t_position) {
	m_position = t_position;
	Notify();
}

void Light::SetColor(const glm::vec3& t_color) {
	m_color = t_color;
	Notify();
}
}