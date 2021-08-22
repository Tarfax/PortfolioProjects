#include "HighScoreState.h"
#include "MainMenuState.h"
#include <Structs/HighScore.h>
#include <string>
#include <Core/EngineData.h>

void HighScoreState::OnEnter() {
	Input::AddInputCallback(BindFunction(HighScoreState::OnEvent, this), SDL_SCANCODE_S);
	Input::AddInputCallback(BindFunction(HighScoreState::OnEvent, this), SDL_SCANCODE_W);

	CreateUI();

	buttons[currentSelectedElement]->SetState(ButtonState::Selected);
	SDL_Rect position = buttons[currentSelectedElement]->GetPosition();
	selectionImage->SetPosition(position.x - selectionImage->GetPosition().w - 15, position.y + position.h / 2 - selectionImage->GetPosition().h / 2);
}

void HighScoreState::CreateUI()
{
	GameObject* canvasGO = new GameObject();
	canvas = canvasGO->AddComponent<Canvas>();

	std::vector<int> scores = HighScore::GetScores();

	std::sort(scores.begin(), scores.end());
	std::reverse(scores.begin(), scores.end());

	float xPosition = SCREEN_WIDTH / 2;
	int startHeightPosition = EngineHelper::GetScreenTitlePosition();
	int heightIncrement = 50;

	highScore = UIFactory::CreateText("High Score", {255, 255, 255, 255}, 128);
	Vector2 position = {GetXMidPosition(*highScore), (float)startHeightPosition};
	highScore->SetPosition(position);
	canvas->AddUIElement(highScore);

	startHeightPosition = SCREEN_HEIGHT / 3;

	for (int i = 0; i < scores.size(); i++) {
		Text* pos1Text = UIFactory::CreateText(std::to_string(i + 1) + ": " + std::to_string(scores[i]), {255, 255, 255, 255}, 64);
		pos1Text->SetPosition(GetXMidPosition(*pos1Text), startHeightPosition + heightIncrement * i);
		canvas->AddUIElement(pos1Text);
	}

	Text* text = UIFactory::CreateText("<-- Back", {255, 255, 255, 255}, 64);
	Button* backButton = UIFactory::CreateButton("res/Sprites/buttonBackgroundNormal.png", "res/Sprites/buttonBackgroundSelected.png", Vector2(300, 435), text, BindFunction(HighScoreState::MainMenu, this));
	backButton->SetPositionAndText({GetXMidPosition(*backButton), (float)(startHeightPosition + heightIncrement * 6)});
	backButton->ListenForInput(SDL_SCANCODE_SPACE);

	canvas->AddUIElement(text);
	buttons.push_back(backButton);
	canvas->AddUIElement(backButton);

	selectionImage = new Image("res/Sprites/ship.png");
	selectionImage->Init();
	selectionImage->SetPosition(Vector2(0, EngineHelper::GetScreenHeightMidPoint()));
	canvas->AddUIElement(selectionImage);
}

void HighScoreState::OnUpdate(float deltaTime) { }

void HighScoreState::OnExit() {
	Input::RemoveInputCallback(BindFunction(HighScoreState::OnEvent, this), SDL_SCANCODE_S);
	Input::RemoveInputCallback(BindFunction(HighScoreState::OnEvent, this), SDL_SCANCODE_W);
	GameObject::Destroy(canvas->gameObject, Predef::Unknown);
}

void HighScoreState::OnEvent(Event& event) {
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<KeyPressedEvent>(BindFunction(HighScoreState::OnKeyPressedEvent, this));
}

bool HighScoreState::OnKeyPressedEvent(KeyPressedEvent& e) {
	if (e.GetKeyCode() == SDL_SCANCODE_W && e.GetRepeatCount() == 0) {
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

	if (e.GetKeyCode() == SDL_SCANCODE_S && e.GetRepeatCount() == 0) {
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

void HighScoreState::MainMenu(KeyPressedEvent& e) {
	SoundCoordinator::PlayEffect("res/SoundFx/menuEnter2.wav");
	gameInstance->ChangeToState<MainMenuState>();
}

float HighScoreState::GetXMidPosition(UIElement& e) {
	return SCREEN_WIDTH / 2 - e.GetSprite()->Size.X / 2;
}