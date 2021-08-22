#include <FSM/MainMenuState.h>

#include <FSM/QuitGameState.h>
#include <FSM/HighScoreState.h>
#include <FSM/GamePlayState.h>
#include <FSM/SettingsState.h>
#include <FSM/CreditsState.h>

#include <Core/EngineData.h>
#include <Structs/Vector2.h>

void MainMenuState::OnEnter() {
	Input::AddInputCallback(BindFunction(MainMenuState::OnEvent, this), SDL_SCANCODE_S);
	Input::AddInputCallback(BindFunction(MainMenuState::OnEvent, this), SDL_SCANCODE_W);

	CreateUI();

	buttons[currentSelectedElement]->SetState(ButtonState::Selected);
	SetNewSelectionImagePosition();

}

void MainMenuState::CreateUI() {
	float buttonHeight = SCREEN_HEIGHT / 3;
	float buttonHeightOffset = 100;

	GameObject* canvasGO = new GameObject();
	canvas = canvasGO->AddComponent<Canvas>();

	gameTitle = UIFactory::CreateText("GTenaroid", {255, 255, 255, 255}, 128);
	Vector2 position = { GetXMidPosition(*gameTitle), EngineHelper::GetScreenTitlePosition()};
	gameTitle->SetPosition(position);
	canvas->AddUIElement(gameTitle);


	Text* startGame = UIFactory::CreateText("Start Game", {255, 255, 255, 255}, 64);
	Button* startGameButton = UIFactory::CreateButton("res/Sprites/buttonBackgroundNormal.png", "res/Sprites/buttonBackgroundSelected.png", Vector2(300, 35), startGame, BindFunction(MainMenuState::StartGame, this));
	startGameButton->SetPositionAndText({GetXMidPosition(*startGameButton), buttonHeight});
	startGameButton->ListenForInput(SDL_SCANCODE_SPACE);

	canvas->AddUIElement(startGame);
	buttons.push_back(startGameButton);
	canvas->AddUIElement(startGameButton);

	Text* settings = UIFactory::CreateText("Settings", {255, 255, 255, 255}, 64);
	Button* settingsButton = UIFactory::CreateButton("res/Sprites/buttonBackgroundNormal.png", "res/Sprites/buttonBackgroundSelected.png", Vector2(300, 135), settings, BindFunction(MainMenuState::Settings, this));
	buttonHeight += buttonHeightOffset;
	settingsButton->SetPositionAndText({GetXMidPosition(*settingsButton), buttonHeight});
	settingsButton->ListenForInput(SDL_SCANCODE_SPACE);

	canvas->AddUIElement(settings);
	buttons.push_back(settingsButton);
	canvas->AddUIElement(settingsButton);

	Text* credits = UIFactory::CreateText("Credits", {255, 255, 255, 255}, 64);
	Button* creditsButton = UIFactory::CreateButton("res/Sprites/buttonBackgroundNormal.png", "res/Sprites/buttonBackgroundSelected.png", Vector2(300, 235), credits, BindFunction(MainMenuState::Credits, this));
	buttonHeight += buttonHeightOffset;
	creditsButton->SetPositionAndText({GetXMidPosition(*creditsButton), buttonHeight});
	creditsButton->ListenForInput(SDL_SCANCODE_SPACE);

	canvas->AddUIElement(credits);
	buttons.push_back(creditsButton);
	canvas->AddUIElement(creditsButton);

	Text* highScore = UIFactory::CreateText("High Score", {255, 255, 255, 255}, 64);
	Button* highScoreButton = UIFactory::CreateButton("res/Sprites/buttonBackgroundNormal.png", "res/Sprites/buttonBackgroundSelected.png", Vector2(300, 335), highScore, BindFunction(MainMenuState::HighScore, this));
	buttonHeight += buttonHeightOffset;
	highScoreButton->SetPositionAndText({GetXMidPosition(*highScoreButton), buttonHeight});
	highScoreButton->ListenForInput(SDL_SCANCODE_SPACE);

	canvas->AddUIElement(highScore);
	buttons.push_back(highScoreButton);
	canvas->AddUIElement(highScoreButton);

	Text* quitGame = UIFactory::CreateText("Quit Game", {255, 255, 255, 255}, 64);
	Button* quitGameButton = UIFactory::CreateButton("res/Sprites/buttonBackgroundNormal.png", "res/Sprites/buttonBackgroundSelected.png", Vector2(300, 435), quitGame, BindFunction(MainMenuState::QuitGame, this));
	buttonHeight += buttonHeightOffset;
	quitGameButton->SetPositionAndText({GetXMidPosition(*quitGameButton), buttonHeight});
	quitGameButton->ListenForInput(SDL_SCANCODE_SPACE);

	canvas->AddUIElement(quitGame);
	buttons.push_back(quitGameButton);
	canvas->AddUIElement(quitGameButton);

	selectionImage = new Image("res/Sprites/ship.png");
	selectionImage->Init();
	selectionImage->SetPosition(Vector2(0, buttons[0]->GetPosition().y));
	canvas->AddUIElement(selectionImage);
}

float MainMenuState::GetXMidPosition(UIElement& e) {
	return SCREEN_WIDTH / 2 - e.GetSprite()->Size.X / 2;
}

void MainMenuState::OnUpdate(float deltaTime) {
	if (setNewSelectionImagePosition == true) {
		LerpToNewPosition(deltaTime);
	}
}

void MainMenuState::LerpToNewPosition(float deltaTime) {
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



void MainMenuState::OnExit() {
	Input::RemoveInputCallback(BindFunction(MainMenuState::OnEvent, this), SDL_SCANCODE_S);
	Input::RemoveInputCallback(BindFunction(MainMenuState::OnEvent, this), SDL_SCANCODE_W);
	GameObject::Destroy(canvas->gameObject, Predef::Unknown);
}

void MainMenuState::AddCallback(const EventCallbackFunc& callback) {
	onChangeState.EventCallback = callback;
}

void MainMenuState::RemoveCallback(const EventCallbackFunc& callback) {
	onChangeState.EventCallback = nullptr;
}

void MainMenuState::OnEvent(Event& event) {
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<KeyPressedEvent>(BindFunction(MainMenuState::OnKeyPressedEvent, this));
}

bool MainMenuState::OnKeyPressedEvent(KeyPressedEvent& e) {
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

void MainMenuState::SetNewSelectionImagePosition() {
	selectionImageStartPosition = selectionImage->GetPosition();
	selectionImageEndPosition.x = buttons[currentSelectedElement]->GetPosition().x - selectionImage->GetPosition().w - 15;
	selectionImageEndPosition.y = buttons[currentSelectedElement]->GetPosition().y + buttons[currentSelectedElement]->GetPosition().h / 2 - selectionImage->GetPosition().h / 2;
	setNewSelectionImagePosition = true;
	imagePositionTimer = 0.0f;
}

void MainMenuState::StartGame(KeyPressedEvent&) {
	SoundCoordinator::PlayEffect("res/SoundFx/menuEnter2.wav");
	gameInstance->ChangeToState<GamePlayState>();

}

void MainMenuState::Settings(KeyPressedEvent&) {
	SoundCoordinator::PlayEffect("res/SoundFx/menuEnter2.wav");
	gameInstance->ChangeToState<SettingsState>();
}

void MainMenuState::Credits(KeyPressedEvent&) {
	SoundCoordinator::PlayEffect("res/SoundFx/menuEnter2.wav");
	gameInstance->ChangeToState<CreditsState>();

}

void MainMenuState::HighScore(KeyPressedEvent&) {
	SoundCoordinator::PlayEffect("res/SoundFx/menuEnter2.wav");
	gameInstance->ChangeToState<HighScoreState>();
}

void MainMenuState::QuitGame(KeyPressedEvent&) {
	SoundCoordinator::PlayEffect("res/SoundFx/menuEnter2.wav");
	gameInstance->ChangeToState<QuitGameState>();
}
