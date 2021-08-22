#pragma once
#include <FSM/GameState.h>
#include <Core/GameInstance.h>

class SettingsState: public GameState {
public:
	void OnEnter() override;
	void OnUpdate(float deltaTime) override;
	void OnExit() override;





private:
	void CreateUI();
	float GetXMidPosition(UIElement& e);

	void LerpToNewPosition(float deltaTime, float& timer, float time, SDL_Rect startPosition, SDL_Rect endPosition, UIElement* element, bool& setPosition);
	void OnEvent(Event& event);
	bool OnKeyPressedEvent(KeyPressedEvent& e);
	void SetNewSelectionImagePosition();
	void MainMenu(KeyPressedEvent& e);
	void ChangeVolume(KeyPressedEvent& e);
	void SetHandlePosition(Button* image);
	int currentSelectedElement = 0;

	std::vector<Button*> buttons;

	Image* selectionImage;
	SDL_Rect selectionImageStartPosition;
	SDL_Rect selectionImageEndPosition;
	float imagePositionTimer = 0.0f;
	float imagePositionTime = 0.135f;
	bool setNewSelectionImagePosition;

	Image* musicHandle;
	SDL_Rect musicHandleStartPosition;
	SDL_Rect musicHandleEndPosition;
	float musicHandleTimer = 0.0f;
	float musicHandleTime = 0.135f;
	bool setMusicHandlePosition;

	Image* effectHandle;
	SDL_Rect effectHandleStartPosition;
	SDL_Rect effectHandleEndPosition;
	float effectHandleTimer = 0.0f;
	float effectHandleTime = 0.135f;
	bool setEffectHandlePosition;

	Text* volumeText;
	int musicVolumeButtonIndex;
	int musicVolume = 30;

	Text* effectText;
	int effectVolumeButtonIndex;
	int effectVolume = 30;

	Text* settingsTitle;

	Canvas* canvas;
};