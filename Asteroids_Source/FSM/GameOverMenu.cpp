#include "GameOverMenu.h"
#include "SoundSystem/SoundCoordinator.h"
#include <UI/UIFactory.h>
#include <string>
#include <Core/EngineData.h>
#include <Structs/HighScore.h>

void GameOverMenu::Init(int score) {
	this->score = score;

	HighScore::SetScore(score);
	HighScore::SaveScore();

	Input::AddInputCallback(BindFunction(GameOverMenu::OnEvent, this), SDL_SCANCODE_S);
	Input::AddInputCallback(BindFunction(GameOverMenu::OnEvent, this), SDL_SCANCODE_W);

	CreateUI();

	buttons[currentSelectedElement]->SetState(ButtonState::Selected);
	SetNewSelectionImagePosition();
}

void GameOverMenu::CreateUI() {
	GameObject* canvasGO = new GameObject();
	canvas = canvasGO->AddComponent<Canvas>();

	gameOver = UIFactory::CreateText("Game Over!", {255, 255, 255, 255}, 88);
	Vector2 size = gameOver->GetSprite()->Size;
	Vector2 position {EngineHelper::GetScreenWidthMidPoint() - size.X / 2, 100};
	gameOver->SetPosition(position);
	canvas->AddUIElement(gameOver);

	scoreText = UIFactory::CreateText("Score: " + std::to_string(score), {255, 255, 255, 255}, 64);
	size = scoreText->GetSprite()->Size;
	position.Y = 165;
	scoreText->SetPosition(position);
	
	canvas->AddUIElement(scoreText);

	Text* resumeText = UIFactory::CreateText("Restart Game", {255, 255, 255, 255}, 58);
	Button* resumeButton = UIFactory::CreateButton("res/Sprites/buttonBackgroundNormal.png", "res/Sprites/buttonBackgroundSelected.png", Vector2(300, 235), resumeText, BindFunction(GameOverMenu::RestartGame, this));
	size = resumeButton->GetSprite()->Size;
	position.Y = 235;
	resumeButton->SetPositionAndText(position);
	resumeButton->ListenForInput(SDL_SCANCODE_SPACE);

	canvas->AddUIElement(resumeText);
	buttons.push_back(resumeButton);
	canvas->AddUIElement(resumeButton);

	Text* mainMenuText = UIFactory::CreateText("Main Menu", {255, 255, 255, 255}, 64);
	Button* mainMenuButton = UIFactory::CreateButton("res/Sprites/buttonBackgroundNormal.png", "res/Sprites/buttonBackgroundSelected.png", Vector2(300, 335), mainMenuText, BindFunction(GameOverMenu::MainMenuState, this));
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

void GameOverMenu::Update(float deltaTime) {
	if (setNewSelectionImagePosition == true) {
		LerpToNewPosition(deltaTime);
	}
}

void GameOverMenu::LerpToNewPosition(float deltaTime) {
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


void GameOverMenu::Destroy() {
	Input::RemoveInputCallback(BindFunction(GameOverMenu::OnEvent, this), SDL_SCANCODE_S);
	Input::RemoveInputCallback(BindFunction(GameOverMenu::OnEvent, this), SDL_SCANCODE_W);
	GameObject::Destroy(canvas->gameObject, Predef::Unknown);
}

void GameOverMenu::AddCallback(const EventCallbackFunc& callback) {
	onChangeState.EventCallback = callback;

}

void GameOverMenu::RemoveCallback(const EventCallbackFunc& callback) {
	onChangeState.EventCallback = nullptr;

}

void GameOverMenu::OnEvent(Event& event) {
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<KeyPressedEvent>(BindFunction(GameOverMenu::OnKeyPressedEvent, this));
}

bool GameOverMenu::OnKeyPressedEvent(KeyPressedEvent& e) {
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

void GameOverMenu::SetNewSelectionImagePosition() {
	selectionImageStartPosition = selectionImage->GetPosition();
	selectionImageEndPosition.x = buttons[currentSelectedElement]->GetPosition().x - selectionImage->GetPosition().w - 15;
	selectionImageEndPosition.y = buttons[currentSelectedElement]->GetPosition().y + buttons[currentSelectedElement]->GetPosition().h / 2 - selectionImage->GetPosition().h / 2;
	setNewSelectionImagePosition = true;
	imagePositionTimer = 0.0f;
}

void GameOverMenu::RestartGame(KeyPressedEvent& event) {
	MenuRestartGameEvent e(NULL);
	FireEvent(e);
}

void GameOverMenu::MainMenuState(KeyPressedEvent& event) {
	MenuMainMenuEvent e(NULL);
	FireEvent(e);
}

void GameOverMenu::FireEvent(Event& event) {
	SoundCoordinator::PlayEffect("res/SoundFx/menuEnter2.wav");
	onChangeState.EventCallback(event);
}
