#include <FSM/GamePlayState.h>

#include <FSM/PauseMenu.h>
#include <FSM/MainMenuState.h>
#include <FSM/GameOverMenu.h>

#include <Core/Engine.h>
#include <Core/GameInstance.h>

#include <Component/Behaviour/PlayerController.h>

#include <Component/Behaviour/Asteroid.h>
#include <Component/ParticleSystem.h>
#include <Component/Behaviour/UFO.h>
#include <Component/Core/BoxCollider2D.h>


#include <Objects/GameObject.h>

#include <EventSystem/MenuEvent.h>

#include <FactorySystem/Factory.h>

void GamePlayState::OnEnter() {
	Input::AddInputCallback(BindFunction(GamePlayState::OnEvent, this), SDL_SCANCODE_ESCAPE);

	Asteroid::AddOnDestroyCallback(BindFunction(GamePlayState::OnEvent, this));
	ParticleSystem::AddOnDestroyCallback(BindFunction(GamePlayState::OnEvent, this));
	UFO::AddOnDestroyCallback(BindFunction(GamePlayState::OnEvent, this));
	PlayerController::AddOnDestroyCallback(BindFunction(GamePlayState::OnEvent, this));


	CreatePlayer();

	loadNextLevel = true;
	nextLevelTimer = timeToLoadLevel;

	isPaused = false;
	gameInstance->SetPaused(isPaused);
	gameInstance->SetTimeScale(isPaused ? 0 : 1);

	nextUFOTimer = 5.0f;
	life = 3;
	playerRespawnTimer = playerRespawnTime;
	UFOsalive = 0;

	CreateUI();
}

void GamePlayState::CreateUI()
{
	GameObject* canvasGO = new GameObject();
	canvas = canvasGO->AddComponent<Canvas>();

	scoreText = UIFactory::CreateText("00", {255, 255, 255, 255}, 50);
	scoreText->SetPosition(scoreTextPosition);

	canvas->AddUIElement(scoreText);

	Image* life1 = new Image("res/Sprites/ship.png");
	Image* life2 = new Image("res/Sprites/ship.png");
	Image* life3 = new Image("res/Sprites/ship.png");

	life1->Init();
	life2->Init();
	life3->Init();

	lifeImages.push_back(life1);
	lifeImages.push_back(life2);
	lifeImages.push_back(life3);

	for (int i = 0; i < 3; i++) {
		lifeImages[i]->SetPosition(lifeImagesPosition + Vector2(22 * i, 0));
		lifeImages[i]->SetRotation(-90);
		lifeImages[i]->SetScale(0.85f);
	}

	canvas->AddUIElement(life1);
	canvas->AddUIElement(life2);
	canvas->AddUIElement(life3);
}

void GamePlayState::OnUpdate(float deltaTime) {
	if (pauseMenu != nullptr) {
		pauseMenu->Update(deltaTime);
	}

	if (gameOverMenu != nullptr) {
		gameOverMenu->Update(deltaTime);
	}

	if (loadNextLevel == true && isPaused == false) {
		nextLevelTimer -= deltaTime;
		if (nextLevelTimer < 0.0f) {
			CreateLevel(currentLevel++);
			loadNextLevel = false;
			nextLevelTimer = timeToLoadLevel;
			SoundCoordinator::PlayEffect("res/SoundFx/levelBegin2.wav");
		}
	}

	if (loadNextLevel == false && isPaused == false && UFOsalive == 0 && isPlayerAlive == true) {
		nextUFOTimer -= deltaTime;
		if (nextUFOTimer <= 0.0f) {
			SpawnUFO();
		}
	}

	if (isPlayerAlive == false && life > 0) {
		playerRespawnTimer -= deltaTime;
		if (playerRespawnTimer <= 0.0f) {
			CreatePlayer();
			playerRespawnTimer = playerRespawnTime;
		}
	}
}

void GamePlayState::OnExit() {
	Input::RemoveInputCallback(BindFunction(GamePlayState::OnEvent, this), SDL_SCANCODE_ESCAPE);

	Asteroid::RemoveOnDestroyCallback(BindFunction(GamePlayState::OnEvent, this));
	ParticleSystem::RemoveOnDestroyCallback(BindFunction(GamePlayState::OnEvent, this));


	DestroyGameObjectsInScene();

	DestroyPauseMenu();
	DestroyGameOverMenu();

	GameObject::Destroy(canvas->gameObject, Predef::Unknown);
	Factory::Destroy();
	playerTransform = nullptr;
}

void GamePlayState::DestroyGameObjectsInScene() {
	std::map<Predef, std::vector<int>>::iterator it;
	for (it = spawnedObjects.begin(); it != spawnedObjects.end(); it++) {

		std::vector<int>::iterator it2;
		for (it2 = it->second.begin(); it2 != it->second.end(); it2++) {
			GameObject::Destroy(idGameObjects[*it2], idObjectDefinitions[*it2]);
		}
		it->second.clear();
	}
	spawnedObjects.clear();
	idObjectDefinitions.clear();
	idGameObjects.clear();
}

void GamePlayState::AddObjectToList(GameObject* gameObject, Predef predef) {
	idObjectDefinitions[gameObject->id] = predef;
	spawnedObjects[predef].push_back(gameObject->id);
	idGameObjects[gameObject->id] = gameObject;
}

void GamePlayState::RemoveObjectFromList(GameObject* gameObject, Predef predef) {
	idGameObjects.erase(gameObject->id);
	for (int i = 0; i < spawnedObjects[predef].size(); i++) {
		if (spawnedObjects[predef][i] == gameObject->id) {
			spawnedObjects[predef].erase(spawnedObjects[predef].begin() + i);
			break;
		}
	}
	idGameObjects.erase(gameObject->id);
}

void GamePlayState::DestroyPauseMenu()
{
	if (pauseMenu != nullptr) {
		pauseMenu->Destroy();
		delete pauseMenu;
		pauseMenu = nullptr;
	}
}

void GamePlayState::DestroyGameOverMenu()
{
	if (gameOverMenu != nullptr) {
		gameOverMenu->Destroy();
		delete gameOverMenu;
		gameOverMenu = nullptr;
	}
}

void GamePlayState::CreatePlayer() {
	GameObject* gameObject = Factory::Create<PlayerController>(Predef::Player);
	playerTransform = gameObject->GetComponent<Transform>();
	playerTransform->Position() = Vector2(SCREEN_WIDTH / 2 + gameObject->GetComponent<BoxCollider2D>()->GetBounds().w / 2 , SCREEN_HEIGHT / 2 + gameObject->GetComponent<BoxCollider2D>()->GetBounds().h / 2);
	isPlayerAlive = true;
	AddObjectToList(gameObject, Predef::Player);
}

void GamePlayState::SpawnUFO() {
	nextUFOTimer = Mathf::RandomFloat(5, 15);
	GameObject* gameObject = Factory::Create<UFO>(Predef::UFO);
	UFOsalive++;
	AddObjectToList(gameObject, Predef::UFO);
}

void GamePlayState::CreateLevel(int level) {
	int height = SCREEN_HEIGHT / 3;

	for (int i = 0; i < level + 4; i++) {
		GameObject* gameObject = Factory::Create<Asteroid>(Predef::Asteroid_Lvl1);
		Transform* transform = gameObject->GetComponent<Transform>();
		if (i % 2 == 0) {
			transform->Position() = Vector2(32, height * (i + 1));
		}
		else {
			transform->Position() = Vector2(SCREEN_WIDTH - 32, height * (i + 1));
		}

		asteroidsInPlay++;
		AddObjectToList(gameObject, Predef::Asteroid_Lvl1);
	}

	for (int i = life; i < 3; i++) {
		canvas->AddUIElement(lifeImages[i]);
	}

	life = 3;
}


void GamePlayState::SetScore(int score) {
	this->score += score;
	canvas->RemoveUIElement(scoreText);
	scoreText->Destroy();
	delete scoreText;
	scoreText = UIFactory::CreateText(std::to_string(this->score), {255, 255, 255, 255}, 50);
	scoreText->SetPosition(scoreTextPosition);
	canvas->AddUIElement(scoreText);
}


void GamePlayState::OnEvent(Event& e) {
	EventDispatcher dispatcher(e);
	if (dispatcher.Dispatch<AsteroidDestroyedEvent>(BindFunction(GamePlayState::OnAsteroidDestroyed, this)) == true) {
		return;
	}

	if (dispatcher.Dispatch<ParticleDestroyedEvent>(BindFunction(GamePlayState::OnParticleDestroyed, this)) == true) {
		return;
	}

	if (dispatcher.Dispatch<UFODestroyedEvent>(BindFunction(GamePlayState::OnUFODestroyed, this)) == true) {
		return;
	}

	if (dispatcher.Dispatch<PlayerDestroyedEvent>(BindFunction(GamePlayState::OnPlayerDestroyed, this)) == true) {
		return;
	}

	if (dispatcher.Dispatch<KeyPressedEvent>(BindFunction(GamePlayState::OnKeyPressed, this)) == true) {
		return;
	}

	if (dispatcher.Dispatch<MenuResumeGameEvent>(BindFunction(GamePlayState::OnResumeGame, this)) == true) {
		return;
	}

	if (dispatcher.Dispatch<MenuMainMenuEvent>(BindFunction(GamePlayState::OnMainMenu, this)) == true) {
		return;
	}

	if (dispatcher.Dispatch<MenuRestartGameEvent>(BindFunction(GamePlayState::OnRestartGame, this)) == true) {
		return;
	}
}


bool GamePlayState::OnKeyPressed(KeyPressedEvent& e) {
	if (e.GetKeyCode() == SDL_SCANCODE_ESCAPE && e.GetRepeatCount() == 0 && life > 0) {
		isPaused = !isPaused;
		gameInstance->SetPaused(isPaused);
		gameInstance->SetTimeScale(isPaused ? 0 : 1);
		if (isPaused == true) {
			CreatePauseMenu();
		}
		else {
			DestroyPauseMenu();
		}
	}
	return true;
}

void GamePlayState::CreatePauseMenu() {
	pauseMenu = new PauseMenu();
	pauseMenu->Init();
	pauseMenu->AddCallback(BindFunction(GamePlayState::OnEvent, this));
}

void GamePlayState::CreateGameOverMenu() {
	gameOverMenu = new GameOverMenu();
	gameOverMenu->Init(score);
	gameOverMenu->AddCallback(BindFunction(GamePlayState::OnEvent, this));
}

bool GamePlayState::OnPlayerDestroyed(PlayerDestroyedEvent& e) {
	if (idGameObjects.count(e.gameObject->id) == 0) {
		return false;
	}

	life--;
	isPlayerAlive = false;
	GameObject* gameObject = Factory::Create<ParticleSystem>(Predef::AsteroidExplosion);
	gameObject->GetComponent<Transform>()->Position() = e.collider->GetOrigin();
	AddObjectToList(gameObject, Predef::AsteroidExplosion);

	SoundCoordinator::PlayEffect("res/SoundFx/playerDeath.wav");

	Image* image = lifeImages[life];
	canvas->RemoveUIElement(image);

	if (spawnedObjects.count(Predef::UFO) == 1) {
		if (spawnedObjects[Predef::UFO].size() > 0) {
			GameObject::Destroy(idGameObjects[spawnedObjects[Predef::UFO][0]], Predef::UFO);
		}
	}

	if (life == 0) {
		CreateGameOverMenu();
	}
}

bool GamePlayState::OnAsteroidDestroyed(AsteroidDestroyedEvent& e) {
	if (idGameObjects.count(e.gameObject->id) == 0) {
		return false;
	}

	RemoveObjectFromList(e.gameObject, e.predef);
	asteroidsInPlay--;

	BoxCollider2D* collider = e.collider;

	if (e.Level == 1) {
		for (int i = 0; i < 2; i++) {
			GameObject* gameObject = Factory::Create<Asteroid>(Predef::Asteroid_Lvl2);
			Transform* transform = gameObject->GetComponent<Transform>();
			transform->Position() = collider->GetOrigin();
			asteroidsInPlay++;
			AddObjectToList(gameObject, Predef::Asteroid_Lvl2);

		}

		//Add score
		SetScore(20);
	}
	else if (e.Level == 2) {
		BoxCollider2D* collider = e.gameObject->GetComponent<BoxCollider2D>();
		for (int i = 0; i < 2; i++) {
			GameObject* gameObject = Factory::Create<Asteroid>(Predef::Asteroid_Lvl3);
			Transform* transform = gameObject->GetComponent<Transform>();
			transform->Position() = collider->GetOrigin();
			asteroidsInPlay++;

			AddObjectToList(gameObject, Predef::Asteroid_Lvl3);
		}

		//Add score
		SetScore(100);
	}
	else {

		//Add score
		SetScore(100);
	}

	CheckWinCondition();

	GameObject* gameObject = Factory::Create<ParticleSystem>(Predef::AsteroidExplosion);
	gameObject->GetComponent<Transform>()->Position() = collider->GetOrigin();
	AddObjectToList(gameObject, Predef::AsteroidExplosion);

	SoundCoordinator::PlayEffect("res/SoundFx/explosion.wav");
	return true;
}

void GamePlayState::CheckWinCondition() {
	if (asteroidsInPlay == 0 && UFOsalive == 0) {
		std::cout << "YOU OWN!!! CHOO CHOO!!" << std::endl;
		loadNextLevel = true;
	}
}

bool GamePlayState::OnParticleDestroyed(ParticleDestroyedEvent& e) {
	if (idGameObjects.count(e.gameObject->id) == 0) {
		return false;
	}
	RemoveObjectFromList(e.gameObject, e.predef);
	return true;
}

bool GamePlayState::OnUFODestroyed(UFODestroyedEvent& e) {
	if (idGameObjects.count(e.gameObject->id) == 0) {
		return false;
	}

	RemoveObjectFromList(e.gameObject, e.predef);

	UFOsalive--;
	if (e.isDeathByPlayer == true) {
		SetScore(1000);
	}

	CheckWinCondition();

	GameObject* gameObject = Factory::Create<ParticleSystem>(Predef::UFOExplosion);
	gameObject->GetComponent<Transform>()->Position() = e.collider->GetOrigin();
	AddObjectToList(gameObject, Predef::UFOExplosion);

	return true;
}

bool GamePlayState::OnResumeGame(MenuResumeGameEvent& e) {
	isPaused = !isPaused;
	gameInstance->SetPaused(isPaused);
	gameInstance->SetTimeScale(isPaused ? 0 : 1);
	DestroyPauseMenu();
	SoundCoordinator::PlayEffect("res/SoundFx/menuEnter2.wav");
	return true;
}

bool GamePlayState::OnMainMenu(MenuMainMenuEvent& e) {
	SoundCoordinator::PlayEffect("res/SoundFx/menuEnter2.wav");
	gameInstance->ChangeToState<MainMenuState>();
	return true;
}

bool GamePlayState::OnRestartGame(MenuRestartGameEvent& e) {
	DestroyGameOverMenu();

	SoundCoordinator::PlayEffect("res/SoundFx/menuEnter2.wav");
	gameInstance->ChangeToState<GamePlayState>();
	return true;
}

