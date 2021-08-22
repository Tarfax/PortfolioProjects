#pragma once
#include <string>
#include <functional>


enum class EventType {
	None = 0,
	KeyPressed, KeyReleased,
	WindowClose,
	ObjectSpawned, ObjectDestroyed, ParticleDestroyed, AsteroidDestroyed, UFODestroyed, PlayerDestroyed,
	MenuStartGame, MenuCredits, MenuSettings, MenuHighScore, MenuQuitGame, MenuResumeGame, MenuRestartGame, MenuMainMenu
};

enum EventCategory {
	None = 0,
	EventCategoryInput = 1,
	EventCategoryKeyboard = 2,
	EventCategoryEngine = 3,
	EventCategoryGameObject = 4,
	EventCategoryMenu = 5,
};

/// <summary>
/// Creates a reference to a member function
/// </summary>
/// <param name="x">The function</param>
/// <param name="y">The instance of the class</param>
#define BindFunction(x, y) std::bind(&x, y, std::placeholders::_1)

#define EventClassType(type) static EventType GetStaticType() { return EventType::##type;} \
							 virtual EventType GetEventType() const override { return GetStaticType(); } \
							 virtual const char* GetName() const override { return #type; }

#define EventClassCategory(category) virtual int GetCategoryFlags() const override { return category; }

class Event {
	friend class EventDispatcher;
public:
	virtual EventType GetEventType() const = 0;
	virtual const char* GetName() const = 0;
	virtual int GetCategoryFlags() const = 0;
	virtual std::string ToString() const { return GetName(); }

	inline bool IsInCategory(EventCategory category) {
		return GetCategoryFlags() & category;
	}
protected: 
	bool handled = false;
};

class EventDispatcher {
	template <typename T>
	using EventFunction = std::function<bool(T&)>;

public: 
	EventDispatcher(Event& event) 
		: event(event) { }

	template <typename T>
	bool Dispatch(EventFunction<T> func) {
		if (event.GetEventType() == T::GetStaticType()) {
			event.handled = func(*(T*)&event);
			return true;
		}
		return false;
	}

private :
	Event& event;

};

inline std::ostream& operator<<(std::ostream& os, const Event& e) {
	return os << e.ToString();
}
