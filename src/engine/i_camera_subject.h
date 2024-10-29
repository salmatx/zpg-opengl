#pragma once

#include "i_camera_observer.h"

class ICameraObserver;

namespace engine{

class ICameraSubject{
public:
	virtual ~ICameraSubject() = default;

	virtual void Attach(ICameraObserver* observer) = 0;
	virtual void Detach(ICameraObserver* observer) = 0;
	virtual void Notify() = 0;
};
}