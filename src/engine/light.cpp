#include "light.h"

namespace engine {
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
