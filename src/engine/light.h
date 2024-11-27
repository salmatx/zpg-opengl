#pragma once

#include "i_light_observer.h"

#include "epch.h"

namespace engine {

class Light {
public:
	Light(LightType_t t_type);
	virtual ~Light() = default;

	virtual void Notify() = 0;
	virtual void InitLight() = 0;

	void Attach(ILightObserver* t_observer);
	void Detach(ILightObserver* t_observer);

	std::list<ILightObserver*>& GetLightObservers();
	[[nodiscard]] int GetId() const {return m_id;}
	[[nodiscard]] LightType_t GetType() const {return m_type;}

private:
	static int s_id;

	LightType_t m_type;
	std::list<ILightObserver*> m_observers;
	int m_id;
};
}