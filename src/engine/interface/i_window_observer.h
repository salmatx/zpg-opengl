#pragma once

#include "i_window_subject.h"

namespace engine {
enum class EventType;

class IWindowObserver {
public:
	virtual ~IWindowObserver() = default;

	virtual void Update(EventType t_event, const std::shared_ptr<const void>& t_event_data) = 0;
};
}
