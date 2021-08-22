#pragma once
#include <EventSystem/Event.h>
#include <sstream>
//#include <UI/MainMenu.h>

class GameInstance;

class MenuEvent: public Event {
public:
	GameInstance* mainMenu;

	EventClassCategory(EventCategoryMenu)
protected:
	MenuEvent(GameInstance* mainMenu)
		: mainMenu(mainMenu) { }

};

class MenuStartGameEvent: public MenuEvent {
public:
	MenuStartGameEvent(GameInstance* mainMenu)
		: MenuEvent(mainMenu)
	{ }

	std::string ToString() const override {
		std::stringstream ss;
		ss << "MenuStartGameEvent was pressed";
		return ss.str();
	}

	EventClassType(MenuStartGame)

};

class MenuSettingsEvent: public MenuEvent {
public:
	MenuSettingsEvent(GameInstance* mainMenu)
		: MenuEvent(mainMenu)
	{ }

	std::string ToString() const override {
		std::stringstream ss;
		ss << "MenuSettingsEvent was pressed";
		return ss.str();
	}

	EventClassType(MenuSettings)
};

class MenuCreditsEvent: public MenuEvent {
public:
	MenuCreditsEvent(GameInstance* mainMenu)
		: MenuEvent(mainMenu)
	{ }

	std::string ToString() const override {
		std::stringstream ss;
		ss << "MenuCreditsEvent was pressed";
		return ss.str();
	}

	EventClassType(MenuCredits)
};

class MenuHighScoreEvent: public MenuEvent {
public:
	MenuHighScoreEvent(GameInstance* mainMenu)
		: MenuEvent(mainMenu)
	{ }

	std::string ToString() const override {
		std::stringstream ss;
		ss << "MenuHighScoreEvent was pressed";
		return ss.str();
	}

	EventClassType(MenuHighScore)

};

class MenuQuitGameEvent: public MenuEvent {
public:
	MenuQuitGameEvent(GameInstance* mainMenu)
		: MenuEvent(mainMenu)
	{ }

	std::string ToString() const override {
		std::stringstream ss;
		ss << "MenuQuitGameEvent was pressed";
		return ss.str();
	}

	EventClassType(MenuQuitGame)
};

class MenuResumeGameEvent: public MenuEvent {
public:
	MenuResumeGameEvent(GameInstance* mainMenu)
		: MenuEvent(mainMenu)
	{ }

	std::string ToString() const override {
		std::stringstream ss;
		ss << "MenuResumeGameEvent was pressed";
		return ss.str();
	}

	EventClassType(MenuResumeGame)
};

class MenuRestartGameEvent : public MenuEvent {
public:
	MenuRestartGameEvent(GameInstance * mainMenu)
		: MenuEvent(mainMenu)
	{ }

	std::string ToString() const override {
		std::stringstream ss;
		ss << "MenuRestartGameEvent was pressed";
		return ss.str();
	}

	EventClassType(MenuRestartGame)
};

class MenuMainMenuEvent: public MenuEvent {
public:
	MenuMainMenuEvent(GameInstance* mainMenu)
		: MenuEvent(mainMenu)
	{ }

	std::string ToString() const override {
		std::stringstream ss;
		ss << "MenuMainMenuEvent was pressed";
		return ss.str();
	}

	EventClassType(MenuMainMenu)
};