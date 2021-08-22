#pragma once
#include <FSM/GameState.h>

class CreditsState: public GameState {
public:
	void OnEnter() override;
	void CreateUI();
	void OnUpdate(float deltaTime) override;
	void OnExit() override;

	std::string ToString() {
		return "CreditsState";
	};

private:
	void OnEvent(Event& event);
	bool OnKeyPressedEvent(KeyPressedEvent& e);

	void MainMenu(KeyPressedEvent& e);

	float GetXMidPosition(UIElement& e);

	int currentSelectedElement = 0;

	Text* credits;
	std::vector<Button*> buttons;
	Image* selectionImage;

	Canvas* canvas;
};