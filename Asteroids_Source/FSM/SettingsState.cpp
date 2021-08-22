#include "SettingsState.h"
#include <Core/EngineData.h>
#include "MainMenuState.h"


void SettingsState::OnEnter() {
	effectVolume = SoundCoordinator::GetEffectVolume();
	musicVolume = SoundCoordinator::GetMusicVolume();

	Input::AddInputCallback(BindFunction(SettingsState::OnEvent, this), SDL_SCANCODE_S);
	Input::AddInputCallback(BindFunction(SettingsState::OnEvent, this), SDL_SCANCODE_W);

	CreateUI();

	buttons[currentSelectedElement]->SetState(ButtonState::Selected);
	SetNewSelectionImagePosition();
	//setMusicHandlePosition = true;
	//setEffectHandlePosition = true;
}

void SettingsState::CreateUI() {
	GameObject* canvasGO = new GameObject();
	canvas = canvasGO->AddComponent<Canvas>();

	int startHeightPosition = EngineHelper::GetScreenTitlePosition();
	int heightIncrement = 100;

	settingsTitle = UIFactory::CreateText("Settings", {255, 255, 255, 255}, 128);
	Vector2 position = {GetXMidPosition(*settingsTitle), (float)startHeightPosition};
	settingsTitle->SetPosition(position);
	canvas->AddUIElement(settingsTitle);

	volumeText = UIFactory::CreateText("Music Volume: ", {255, 255, 255, 255}, 64);
	volumeText->SetPosition(GetXMidPosition(*volumeText), startHeightPosition + heightIncrement * 2);
	Button* musicButton = UIFactory::CreateButton("res/Sprites/volumeSlider.png", "res/Sprites/volumeSlider.png", Vector2(GetXMidPosition(*volumeText) + volumeText->GetSprite()->Size.X + 15, startHeightPosition + heightIncrement * 2), nullptr, BindFunction(SettingsState::ChangeVolume, this));
	musicButton->ListenForInput(SDL_SCANCODE_A);
	musicButton->ListenForInput(SDL_SCANCODE_D);
	musicVolumeButtonIndex = buttons.size();
	buttons.push_back(musicButton);

	musicHandle = new Image("res/Sprites/volumeHandle.png");
	musicHandle->Init();

	int start = musicButton->GetPosition().x;
	int end = musicButton->GetPosition().w + start;
	int intervall = (end - start) / 10;
	musicHandle->SetPosition({(float)musicButton->GetPosition().x + (intervall * (musicVolume / 10)), (float)musicButton->GetPosition().y});

	canvas->AddUIElement(volumeText);
	canvas->AddUIElement(musicButton);
	canvas->AddUIElement(musicHandle);

	effectText = UIFactory::CreateText("Effect Volume: ", {255, 255, 255, 255}, 64);
	effectText->SetPosition(GetXMidPosition(*effectText), startHeightPosition + heightIncrement * 3);
	Button* effectButton = UIFactory::CreateButton("res/Sprites/volumeSlider.png", "res/Sprites/volumeSlider.png", Vector2(GetXMidPosition(*volumeText) + volumeText->GetSprite()->Size.X + 15, startHeightPosition + heightIncrement * 3), nullptr, BindFunction(SettingsState::ChangeVolume, this));
	effectButton->ListenForInput(SDL_SCANCODE_A);
	effectButton->ListenForInput(SDL_SCANCODE_D);
	effectVolumeButtonIndex = buttons.size();
	buttons.push_back(effectButton);

	effectHandle = new Image("res/Sprites/volumeHandle.png");
	effectHandle->Init();

	start = effectButton->GetPosition().x;
	end = effectButton->GetPosition().w + start;
	intervall = (end - start) / 10;
	effectHandle->SetPosition({(float)effectButton->GetPosition().x + (intervall * (effectVolume / 10)), (float)effectButton->GetPosition().y});

	canvas->AddUIElement(effectText);
	canvas->AddUIElement(effectButton);
	canvas->AddUIElement(effectHandle);

	startHeightPosition = SCREEN_HEIGHT / 3;

	Text* text = UIFactory::CreateText("<-- Back", {255, 255, 255, 255}, 64);
	Button* backButton = UIFactory::CreateButton("res/Sprites/buttonBackgroundNormal.png", "res/Sprites/buttonBackgroundSelected.png", Vector2(300, 435), text, BindFunction(SettingsState::MainMenu, this));
	backButton->SetPositionAndText({GetXMidPosition(*backButton), (float)(startHeightPosition + 300)});
	backButton->ListenForInput(SDL_SCANCODE_SPACE);

	buttons.push_back(backButton);
	canvas->AddUIElement(text);
	canvas->AddUIElement(backButton);


	selectionImage = new Image("res/Sprites/ship.png");
	selectionImage->Init();
	selectionImage->SetPosition(Vector2(0, buttons[0]->GetPosition().y));
	canvas->AddUIElement(selectionImage);
}


void SettingsState::OnUpdate(float deltaTime) {
	if (setNewSelectionImagePosition == true) {
		LerpToNewPosition(deltaTime, imagePositionTimer, imagePositionTime, selectionImageStartPosition, selectionImageEndPosition, selectionImage, setNewSelectionImagePosition);
	}

	if (setMusicHandlePosition == true) {
		LerpToNewPosition(deltaTime, musicHandleTimer, musicHandleTime, musicHandleStartPosition, musicHandleEndPosition, musicHandle, setMusicHandlePosition);
	}

	if (setEffectHandlePosition == true) {
		LerpToNewPosition(deltaTime, effectHandleTimer, effectHandleTime, effectHandleStartPosition, effectHandleEndPosition, effectHandle, setEffectHandlePosition);
	}
}

void SettingsState::LerpToNewPosition(float deltaTime, float& timer, float duration, SDL_Rect startPosition, SDL_Rect endPosition, UIElement* element, bool& setPosition) {
	timer += deltaTime;
	Vector2 newPosition;
	float time = timer / duration;
	time = (time * time) * (3.0f - (2.0f * time));

	newPosition.X = Mathf::Lerp(startPosition.x, endPosition.x, time);
	newPosition.Y = Mathf::Lerp(startPosition.y, endPosition.y, time);
	element->SetPosition(newPosition);

	if ((timer / duration) >= 1.0f) {
		setPosition = false;
		timer = 0.0f;
		element->SetPosition(endPosition.x, endPosition.y);
	}
}

void SettingsState::OnExit() {
	Input::RemoveInputCallback(BindFunction(SettingsState::OnEvent, this), SDL_SCANCODE_S);
	Input::RemoveInputCallback(BindFunction(SettingsState::OnEvent, this), SDL_SCANCODE_W);
	GameObject::Destroy(canvas->gameObject, Predef::Unknown);
}

void SettingsState::OnEvent(Event& event) {
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<KeyPressedEvent>(BindFunction(SettingsState::OnKeyPressedEvent, this));
}

bool SettingsState::OnKeyPressedEvent(KeyPressedEvent& e) {
	if (e.GetKeyCode() == SDL_SCANCODE_W && e.GetRepeatCount() == 0) {
		buttons[currentSelectedElement]->SetState(ButtonState::Normal);

		currentSelectedElement--;
		if (currentSelectedElement < 0) {
			currentSelectedElement = buttons.size() - 1;
		}
		buttons[currentSelectedElement]->SetState(ButtonState::Selected);

		SetNewSelectionImagePosition();
		SoundCoordinator::PlayEffect("res/SoundFx/menuSelection.wav");
	}

	if (e.GetKeyCode() == SDL_SCANCODE_S && e.GetRepeatCount() == 0) {
		buttons[currentSelectedElement]->SetState(ButtonState::Normal);

		currentSelectedElement++;
		if (currentSelectedElement > buttons.size() - 1) {
			currentSelectedElement = 0;
		}
		buttons[currentSelectedElement]->SetState(ButtonState::Selected);

		SetNewSelectionImagePosition();
		SoundCoordinator::PlayEffect("res/SoundFx/menuSelection.wav");
	}

	return true;
}

void SettingsState::SetNewSelectionImagePosition() {
	selectionImageStartPosition = selectionImage->GetPosition();

	if (currentSelectedElement == musicVolumeButtonIndex) {
		selectionImageEndPosition.x = volumeText->GetPosition().x - selectionImage->GetPosition().w - 15;
	}
	else if (currentSelectedElement == effectVolumeButtonIndex) {
		selectionImageEndPosition.x = effectText->GetPosition().x - selectionImage->GetPosition().w - 15;
	}
	else {
		selectionImageEndPosition.x = buttons[currentSelectedElement]->GetPosition().x - selectionImage->GetPosition().w - 15;
	}

	selectionImageEndPosition.y = buttons[currentSelectedElement]->GetPosition().y + buttons[currentSelectedElement]->GetPosition().h / 2 - selectionImage->GetPosition().h / 2;
	setNewSelectionImagePosition = true;
	imagePositionTimer = 0.0f;
}

void SettingsState::MainMenu(KeyPressedEvent& e) {
	SoundCoordinator::PlayEffect("res/SoundFx/menuEnter2.wav");
	gameInstance->ChangeToState<MainMenuState>();
}

void SettingsState::ChangeVolume(KeyPressedEvent& e) {
	if (e.GetKeyCode() == SDL_SCANCODE_D) {
		if (currentSelectedElement == musicVolumeButtonIndex) {
			musicVolume += 10;
			if (musicVolume > 100) {
				musicVolume = 100;
			}
		}
		else {
			effectVolume += 10;
			if (effectVolume > 100) {
				effectVolume = 100;
			}
		}
	}

	else if (e.GetKeyCode() == SDL_SCANCODE_A) {
		if (currentSelectedElement == musicVolumeButtonIndex) {
			musicVolume -= 10;
			if (musicVolume < 0) {
				musicVolume = 0;
			}
		}
		else {
			effectVolume -= 10;
			if (effectVolume < 0) {
				effectVolume = 0;
			}

		}

	}
	SoundCoordinator::SetEffectVolume(effectVolume);
	SoundCoordinator::SetMusicVolume(musicVolume);
	SetHandlePosition(buttons[currentSelectedElement]);

	SoundCoordinator::PlayEffect("res/SoundFx/menuEnter2.wav");
}

void SettingsState::SetHandlePosition(Button* button) {
	SDL_Rect imagePosition = button->GetPosition();

	int start = imagePosition.x;
	int end = imagePosition.w + start;
	int intervall = (end - start) / 10;

	if (currentSelectedElement == musicVolumeButtonIndex) {
		musicHandleStartPosition = musicHandle->GetPosition();
		musicHandleEndPosition.x = intervall * (musicVolume / 10) + start;
		if (musicVolume == 100) {
			musicHandleEndPosition.x -= 8;
		}
		musicHandleEndPosition.y = musicHandleStartPosition.y;
		setMusicHandlePosition = true;
		musicHandleTimer = 0;
	}
	else {
		effectHandleStartPosition = effectHandle->GetPosition();
		effectHandleEndPosition.x = intervall * (effectVolume / 10) + start;
		if (effectVolume == 100) {
			effectHandleEndPosition.x -= 8;
		}
		effectHandleEndPosition.y = effectHandleStartPosition.y;
		setEffectHandlePosition = true;
		effectHandleTimer = 0;
	}
}


float SettingsState::GetXMidPosition(UIElement& e) {
	return SCREEN_WIDTH / 2 - e.GetSprite()->Size.X / 2;
}