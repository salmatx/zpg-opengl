#pragma once
#include "i_window_observer.h"


namespace engine {
class IWindowObserver;

enum class EventType {
	key,
	mouse,
	scroll,
	screen_size
};

struct KeyEvent {
	int key;
	int action;
	float time;
};

struct MouseEvent {
	double x_pos;
	double y_pos;
};

struct ScrollEvent {
	double x_offset;
	double y_offset;
};

struct ScreenSizeEvent {
	int width;
	int height;
};


class IWindowSubject {
public:
	virtual ~IWindowSubject() = default;

	virtual void Attach(EventType t_eventType, IWindowObserver* t_observer) = 0;
	virtual void Detach(EventType t_eventType, IWindowObserver* t_observer) = 0;
	virtual void Notify(EventType t_event) const = 0;
};
}
