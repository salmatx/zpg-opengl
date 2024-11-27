#include "light.h"

namespace engine {

int Light::s_id = 0;

Light::Light(LightType_t t_type) : m_id(s_id++), m_type(t_type) {}

void Light::Attach(ILightObserver* t_observer) {
	m_observers.emplace_back(t_observer);
}
void Light::Detach(ILightObserver* t_observer) {
	m_observers.remove(t_observer);
}

std::list<ILightObserver*>& Light::GetLightObservers() {
	return m_observers;
}
}
