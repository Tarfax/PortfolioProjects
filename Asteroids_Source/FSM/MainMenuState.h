#pragma once
#include <FSM/GameState.h>

#include <Component/Core/Canvas.h>
#include <UI/Button.h>
#include <UI/Image.h>
#include <UI/Text.h>
#include <Core/Input.h>
#include <EventSystem/MenuEvent.h>

class MainMenuState : public GameState {
	using EventCallbackFunc = std::function<void(Event&)>;

public:
	MainMenuState() {}
	~MainMenuState() {
		//std::cout << "deleted main menu" << std::endl;
	}

	std::string ToString() { 
		return "MainMenuState";
	};

	void OnEnter() override;
	void OnUpdate(float deltaTime) override;
	void OnExit() override;

	/// <summary>
	/// Warning: Same function can be added several times.
	/// </summary>
	void AddCallback(const EventCallbackFunc& callback);

	/// <summary>
	/// Nothing happens if you try to remove the same method twice.
	/// </summary>
	void RemoveCallback(const EventCallbackFunc& callback);

private:
	void CreateUI();

	float GetXMidPosition(UIElement& e);

	void OnEvent(Event& event);
	bool OnKeyPressedEvent(KeyPressedEvent& e);

	void SetNewSelectionImagePosition();
	void LerpToNewPosition(float deltaTime);

	void StartGame	(KeyPressedEvent&);
	void Settings	(KeyPressedEvent&);
	void Credits	(KeyPressedEvent&);
	void HighScore	(KeyPressedEvent&);
	void QuitGame	(KeyPressedEvent&);

	//void FireEvent(Event& event);

	struct CallbackData {
		CallbackData() { }
		CallbackData(EventCallbackFunc callback): EventCallback(callback) { }

		EventCallbackFunc EventCallback;
	};

	CallbackData onChangeState;

	int currentSelectedElement = 0;

	std::vector<Button*> buttons;
	//std::vector<Text*> texts;

	Image* selectionImage;
	SDL_Rect selectionImageStartPosition;
	SDL_Rect selectionImageEndPosition;
	float imagePositionTimer = 0.0f;
	float imagePositionTime = 0.135f;
	bool setNewSelectionImagePosition;

	Text* gameTitle;

	Canvas* canvas;


};