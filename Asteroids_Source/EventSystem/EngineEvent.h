#pragma once

#include <EventSystem/Event.h>
#include <sstream>

class EngineEvent: public Event {
public:
	EventClassCategory(EventCategoryEngine)
protected:
	EngineEvent() { }
};

class EngineCloseEvent: public EngineEvent {
public:
	EngineCloseEvent() { }
	std::string ToString() const override {
		std::stringstream ss;
		ss << "EngineCloseEvent";
		return ss.str();
	}
	EventClassType(WindowClose)
};