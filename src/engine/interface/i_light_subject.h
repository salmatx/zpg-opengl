#pragma once

#include "i_light_observer.h"

class ILightObserver;

namespace engine{

class ILightSubject {
public:
	virtual ~ILightSubject() = default;

	virtual void Attach(ILightObserver* t_observer) = 0;
	virtual void Detach(ILightObserver* t_observer) = 0;
	virtual void Notify() = 0;
};
}