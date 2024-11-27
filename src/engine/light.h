#pragma once

#include "i_light_observer.h"

#include "epch.h"

namespace engine {

class Light {
public:
	Light() = default;
	virtual ~Light() = default;

	virtual void Notify() = 0;
	virtual void InitLight() = 0;

	void Attach(ILightObserver* t_observer);
	void Detach(ILightObserver* t_observer);

	std::list<ILightObserver*>& GetLightObservers();

private:
	std::list<ILightObserver*> m_observers;
};
}