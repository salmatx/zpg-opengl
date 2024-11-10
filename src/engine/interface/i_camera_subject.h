#pragma once

#include "i_camera_observer.h"

class ICameraObserver;

namespace engine{

class ICameraSubject {
public:
	virtual ~ICameraSubject() = default;

	virtual void Attach(ICameraObserver* t_observer) = 0;
	virtual void Detach(ICameraObserver* t_observer) = 0;
	virtual void Notify() = 0;
};
}