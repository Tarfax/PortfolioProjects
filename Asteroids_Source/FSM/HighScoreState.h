#pragma once
#include <FSM/GameState.h>
#include <Core/GameInstance.h>

#include <UI/Button.h>
#include <UI/Image.h>
#include <UI/Text.h>

class MainMenuState;

class HighScoreState: public GameState {


public:
	void OnEnter() override;
	void CreateUI();
	void OnUpdate(float deltaTime) override;
	void OnExit() override;
	std::string ToString() {
		return "HighScoreState";
	};

private:
	void OnEvent(Event& event);
	bool OnKeyPressedEvent(KeyPressedEvent& e);

	void MainMenu(KeyPressedEvent&);

	float GetXMidPosition(UIElement& e);

	int currentSelectedElement = 0;

	Text* highScore;
	std::vector<Button*> buttons;
	Image* selectionImage;

	Canvas* canvas;
};