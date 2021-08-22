#pragma once

#include <EventSystem/Event.h>
#include <sstream>
#include <SDL.h>

class KeyEvent: public Event {
public:
	inline int GetKeyCode() const { return keyCode; }

	EventClassCategory(EventCategoryInput | EventCategoryKeyboard)
protected:
	KeyEvent(SDL_Scancode keyCode)
		: keyCode(keyCode) { }

	SDL_Scancode keyCode;
};

class KeyPressedEvent: public KeyEvent {
public:
	KeyPressedEvent(SDL_Scancode keyCode, int repeatCount, float deltaTime)
		: KeyEvent(keyCode), repeatCount(repeatCount), deltaTime(deltaTime) { }

	inline int GetRepeatCount() const { return repeatCount; }
	inline float GetDeltaTime() const { return deltaTime; }
	std::string ToString() const override {
		std::stringstream ss;
		ss << "KeyPressedEvent: " << keyCode << " (" << repeatCount << " repeats)";
		return ss.str();
	}

	EventClassType(KeyPressed)

private:
	int repeatCount;
	float deltaTime;
};

class KeyReleasedEvent: public KeyEvent {

public:
	KeyReleasedEvent(SDL_Scancode keyCode)
		: KeyEvent(keyCode) { }

	//inline int GetRepeatCount() const { return repeatCount; }
	std::string ToString() const override {
		std::stringstream ss;
		ss << "KeyReleasedEvent: " << keyCode << " is released";
		return ss.str();
	}

	EventClassType(KeyReleased)

private:
	//int repeatCount;

};