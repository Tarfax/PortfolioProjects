#include "PauseMenu.h"
#include "SoundSystem/SoundCoordinator.h"
#include <UI/UIFactory.h>
#include <iostream>
#include <Core/EngineData.h>

void PauseMenu::Init() {
	Input::AddInputCallback(BindFunction(PauseMenu::OnEvent, this), SDL_SCANCODE_S);
	Input::AddInputCallback(BindFunction(PauseMenu::OnEvent, this), SDL_SCANCODE_W);

	CreateUI();

	buttons[currentSelectedElement]->SetState(ButtonState::Selected);
	SetNewSelectionImagePosition();
}

void PauseMenu::CreateUI()
{
	GameObject* canvasGO = new GameObject();
	canvas = canvasGO->AddComponent<Canvas>();

	Text* resumeText = UIFactory::CreateText("Resume Game", {255, 255, 255, 255}, 64);
	Button* resumeButton = UIFactory::CreateButton("res/Sprites/buttonBackgroundNormal.png", "res/Sprites/buttonBackgroundSelected.png", Vector2(300, 235), resumeText, BindFunction(PauseMenu::ResumeGame, this));
	Vector2 size = resumeButton->GetSprite()->Size;
	Vector2 position {EngineHelper::GetScreenWidthMidPoint() - size.X / 2, 235};
	resumeButton->SetPositionAndText(position);
	resumeButton->ListenForInput(SDL_SCANCODE_SPACE);

	canvas->AddUIElement(resumeText);
	buttons.push_back(resumeButton);
	canvas->AddUIElement(resumeButton);

	Text* mainMenuText = UIFactory::CreateText("Main Menu", {255, 255, 255, 255}, 64);
	Button* mainMenuButton = UIFactory::CreateButton("res/Sprites/buttonBackgroundNormal.png", "res/Sprites/buttonBackgroundSelected.png", Vector2(300, 335), mainMenuText, BindFunction(PauseMenu::MainMenuState, this));
	size = mainMenuButton->GetSprite()->Size;
	position.Y = 335;
	mainMenuButton->SetPositionAndText(position);
	mainMenuButton->ListenForInput(SDL_SCANCODE_SPACE);

	canvas->AddUIElement(mainMenuText);
	buttons.push_back(mainMenuButton);
	canvas->AddUIElement(mainMenuButton);

	selectionImage = new Image("res/Sprites/ship.png");
	selectionImage->Init();
	selectionImage->SetPosition(Vector2(0, buttons[0]->GetPosition().y));
	canvas->AddUIElement(selectionImage);
}

void PauseMenu::Update(float deltaTime) { 
	if (setNewSelectionImagePosition == true) {
		LerpToNewPosition(deltaTime);
	}
}

void PauseMenu::LerpToNewPosition(float deltaTime) {
	imagePositionTimer += deltaTime;
	Vector2 newPosition;
	float time = imagePositionTimer / imagePositionTime;
	time = (time * time) * (3.0f - (2.0f * time));

	newPosition.X = Mathf::Lerp(selectionImageStartPosition.x, selectionImageEndPosition.x, time);
	newPosition.Y = Mathf::Lerp(selectionImageStartPosition.y, selectionImageEndPosition.y, time);
	selectionImage->SetPosition(newPosition);

	if ((imagePositionTimer / imagePositionTime) >= 1.0f) {
		setNewSelectionImagePosition = false;
		imagePositionTimer = 0.0f;
		selectionImage->SetPosition(selectionImageEndPosition.x, selectionImageEndPosition.y);
	}
}

void PauseMenu::SetNewSelectionImagePosition() {
	selectionImageStartPosition = selectionImage->GetPosition();
	selectionImageEndPosition.x = buttons[currentSelectedElement]->GetPosition().x - selectionImage->GetPosition().w - 15;
	selectionImageEndPosition.y = buttons[currentSelectedElement]->GetPosition().y + buttons[currentSelectedElement]->GetPosition().h / 2 - selectionImage->GetPosition().h / 2;
	setNewSelectionImagePosition = true;
	imagePositionTimer = 0.0f;
}


void PauseMenu::Destroy() {
	Input::RemoveInputCallback(BindFunction(PauseMenu::OnEvent, this), SDL_SCANCODE_S);
	Input::RemoveInputCallback(BindFunction(PauseMenu::OnEvent, this), SDL_SCANCODE_W);
	GameObject::Destroy(canvas->gameObject, Predef::Unknown);
}

void PauseMenu::AddCallback(const EventCallbackFunc& callback) {
	onChangeState.EventCallback = callback;
}

void PauseMenu::RemoveCallback(const EventCallbackFunc& callback) {
	onChangeState.EventCallback = nullptr;
}

void PauseMenu::OnEvent(Event& event) {
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<KeyPressedEvent>(BindFunction(PauseMenu::OnKeyPressedEvent, this));
}

bool PauseMenu::OnKeyPressedEvent(KeyPressedEvent& e) {
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

void PauseMenu::ResumeGame(KeyPressedEvent&) {
	MenuResumeGameEvent e(NULL);
	FireEvent(e);
}

void PauseMenu::MainMenuState(KeyPressedEvent&) {
	MenuMainMenuEvent e(NULL);
	FireEvent(e);
}

void PauseMenu::FireEvent(Event& event) {
	SoundCoordinator::PlayEffect("res/SoundFx/menuEnter2.wav");
	onChangeState.EventCallback(event);
}