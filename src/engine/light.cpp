#include "light.h"

namespace engine {
Light::Light(const LightParams& t_params)
	: m_type(t_params.type),
	  m_position(t_params.position),
	  m_direction(t_params.direction),
	  m_color(t_params.color),
	  m_intensity(t_params.intensity),
	  m_cutoff(t_params.cutoff),
	  m_outer_cutoff(t_params.outer_cutoff) {}

void Light::SetPosition(const glm::vec3& t_position) {
	m_position = t_position;
	Notify();
}

void Light::SetColor(const glm::vec3& t_color) {
	m_color = t_color;
	Notify();
}

void Light::SetDirection(const glm::vec3& t_direction) {
	m_direction = t_direction;
	Notify();
}

void Light::SetCutoff(float t_cutoff) {
	m_cutoff = t_cutoff;
	Notify();
}

void Light::SetOuterCutoff(float t_outer_cutoff) {
	m_outer_cutoff = t_outer_cutoff;
	Notify();
}

void Light::SetIntensity(float t_intensity) {
	m_intensity = t_intensity;
	Notify();
}

void Light::SetType(LightType t_type) {
	m_type = t_type;
	Notify();
}

void Light::Attach(ILightObserver* t_observer) {
	m_observers.emplace_back(t_observer);
}

void Light::Detach(ILightObserver* t_observer) {
	m_observers.remove(t_observer);
}

void Light::Notify() {
	for (ILightObserver* observer : m_observers) {
		observer->Update(m_position, m_color);
	}
}

void Light::InitLight() {
	Notify();
}
}
