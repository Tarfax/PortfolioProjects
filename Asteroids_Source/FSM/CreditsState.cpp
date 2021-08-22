#include "CreditsState.h"
#include "MainMenuState.h"
#include <Core/GameInstance.h>
#include <Core/EngineData.h>

void CreditsState::OnEnter() {
	Input::AddInputCallback(BindFunction(CreditsState::OnEvent, this), SDL_SCANCODE_DOWN);
	Input::AddInputCallback(BindFunction(CreditsState::OnEvent, this), SDL_SCANCODE_UP);

	CreateUI();

	buttons[currentSelectedElement]->SetState(ButtonState::Selected);
	SDL_Rect position = buttons[currentSelectedElement]->GetPosition();
	selectionImage->SetPosition(position.x - selectionImage->GetPosition().w - 15, position.y + position.h / 2 - selectionImage->GetPosition().h / 2);
}

void CreditsState::CreateUI()
{
	GameObject* canvasGO = new GameObject();
	canvas = canvasGO->AddComponent<Canvas>();

	credits = UIFactory::CreateText("Credits", {255, 255, 255, 255}, 128);
	Vector2 position = {GetXMidPosition(*credits), EngineHelper::GetScreenTitlePosition()};
	credits->SetPosition(position);
	canvas->AddUIElement(credits);

	int titleNameHeightOffset = 120;

	Text* jupiter = UIFactory::CreateText("Jupiter Thulin", {255, 255, 255, 255}, 64);
	Text* jupiterProgrammer = UIFactory::CreateText("Programmer", {255, 255, 255, 255}, 44);

	Text* mikael = UIFactory::CreateText("Mikael Cedergren", {255, 255, 255, 255}, 64);
	Text* mikaelProgrammer = UIFactory::CreateText("Programmer", {255, 255, 255, 255}, 44);
	Text* mikaelSoundDesign = UIFactory::CreateText("Sound Designer", {255, 255, 255, 255}, 44);
	Text* mikaelSoundGraphic = UIFactory::CreateText("Graphic", {255, 255, 255, 255}, 44);

	float jupiterWidthOffset = EngineHelper::GetScreenWidthMidPoint() / 3 + 50;
	float jupiterHeightOffset = EngineHelper::GetScreenTitlePosition() + titleNameHeightOffset;
	jupiter->SetPosition(jupiterWidthOffset, jupiterHeightOffset);
	jupiterProgrammer->SetPosition(jupiterWidthOffset, jupiterHeightOffset + 60);

	canvas->AddUIElement(jupiter);
	canvas->AddUIElement(jupiterProgrammer);

	float mikaelWidthOffset = EngineHelper::GetScreenWidthMidPoint() * 1.25f - 50;
	float mikaelHeightOffset = EngineHelper::GetScreenTitlePosition() + titleNameHeightOffset;
	mikael->SetPosition(mikaelWidthOffset, mikaelHeightOffset);
	mikaelProgrammer->SetPosition(mikaelWidthOffset, mikaelHeightOffset += 60);
	mikaelSoundDesign->SetPosition(mikaelWidthOffset, mikaelHeightOffset += 40);
	mikaelSoundGraphic->SetPosition(mikaelWidthOffset, mikaelHeightOffset += 40);

	canvas->AddUIElement(mikael);
	canvas->AddUIElement(mikaelProgrammer);
	canvas->AddUIElement(mikaelSoundDesign);
	canvas->AddUIElement(mikaelSoundGraphic);

	float heightIncrement = 300;
	float startHeightPosition = SCREEN_HEIGHT / 3;
	Text* backText = UIFactory::CreateText("<-- Back", {255, 255, 255, 255}, 64);
	Button* backButton = UIFactory::CreateButton("res/Sprites/buttonBackgroundNormal.png", "res/Sprites/buttonBackgroundSelected.png", Vector2(300, 435), backText, BindFunction(CreditsState::MainMenu, this));
	backButton->SetPositionAndText({GetXMidPosition(*backButton), (float)(startHeightPosition + heightIncrement)});
	backButton->ListenForInput(SDL_SCANCODE_SPACE);

	canvas->AddUIElement(backText);
	buttons.push_back(backButton);
	canvas->AddUIElement(backButton);

	selectionImage = new Image("res/Sprites/ship.png");
	selectionImage->Init();
	selectionImage->SetPosition(Vector2(0, 35));
	canvas->AddUIElement(selectionImage);
}

void CreditsState::OnUpdate(float deltaTime) { }

void CreditsState::OnExit() {
	Input::RemoveInputCallback(BindFunction(CreditsState::OnEvent, this), SDL_SCANCODE_DOWN);
	Input::RemoveInputCallback(BindFunction(CreditsState::OnEvent, this), SDL_SCANCODE_UP);
	GameObject::Destroy(canvas->gameObject, Predef::Unknown);
}

void CreditsState::OnEvent(Event& event) {
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<KeyPressedEvent>(BindFunction(CreditsState::OnKeyPressedEvent, this));
}

bool CreditsState::OnKeyPressedEvent(KeyPressedEvent& e) {
	if (e.GetKeyCode() == SDL_SCANCODE_UP && e.GetRepeatCount() == 0) {
		buttons[currentSelectedElement]->SetState(ButtonState::Normal);

		currentSelectedElement--;
		if (currentSelectedElement < 0) {
			currentSelectedElement = buttons.size() - 1;
		}
		buttons[currentSelectedElement]->SetState(ButtonState::Selected);
		SDL_Rect position = buttons[currentSelectedElement]->GetPosition();
		selectionImage->SetPosition(position.x - selectionImage->GetPosition().w - 15, position.y + position.h / 2 - selectionImage->GetPosition().h / 2);
		SoundCoordinator::PlayEffect("res/SoundFx/menuSelection.wav");
	}

	if (e.GetKeyCode() == SDL_SCANCODE_DOWN && e.GetRepeatCount() == 0) {
		buttons[currentSelectedElement]->SetState(ButtonState::Normal);

		currentSelectedElement++;
		if (currentSelectedElement > buttons.size() - 1) {
			currentSelectedElement = 0;
		}
		buttons[currentSelectedElement]->SetState(ButtonState::Selected);
		SDL_Rect position = buttons[currentSelectedElement]->GetPosition();
		selectionImage->SetPosition(position.x - selectionImage->GetPosition().w - 15, position.y + position.h / 2 - selectionImage->GetPosition().h / 2);
		SoundCoordinator::PlayEffect("res/SoundFx/menuSelection.wav");
	}

	return true;
}

void CreditsState::MainMenu(KeyPressedEvent& event) {
	SoundCoordinator::PlayEffect("res/SoundFx/menuEnter2.wav");
	gameInstance->ChangeToState<MainMenuState>();
}

float CreditsState::GetXMidPosition(UIElement& e) {
	return SCREEN_WIDTH / 2 - e.GetSprite()->Size.X / 2;
}