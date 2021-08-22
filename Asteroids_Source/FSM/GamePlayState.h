#pragma once
#include <FSM/GameState.h>

#include <vector>
#include <EventSystem/ObjectEvent.h>

#include <FactorySystem/Predef.h>

class MenuMainMenuEvent;
class MenuResumeGameEvent;
class MenuRestartGameEvent;
class PauseMenu;
class GameOverMenu;

class GamePlayState: public GameState {
public:
	void OnEnter() override;
	void CreateUI();
	void OnUpdate(float deltaTime) override;
	void OnExit() override;



private:
	void DestroyGameObjectsInScene();

	void CreatePauseMenu();
	void DestroyPauseMenu();
	void CreateGameOverMenu();
	void DestroyGameOverMenu();

	void OnEvent(Event& e);
	bool OnKeyPressed(KeyPressedEvent& e);


	bool OnPlayerDestroyed(PlayerDestroyedEvent& e);

	bool OnAsteroidDestroyed(AsteroidDestroyedEvent& e);
	bool OnParticleDestroyed(ParticleDestroyedEvent& e);
	bool OnUFODestroyed(UFODestroyedEvent& e);

	void SetScore(int score);
	void CheckWinCondition();

	void CreateLevel(int level);
	void AddObjectToList(GameObject* gameObject, Predef predef);
	void RemoveObjectFromList(GameObject* gameObject, Predef predef);
	void CreatePlayer();
	void SpawnUFO();

	bool OnResumeGame(MenuResumeGameEvent& e);
	bool OnMainMenu(MenuMainMenuEvent& e);
	bool OnRestartGame(MenuRestartGameEvent& e);

	int asteroidsInPlay = 0;
	int currentLevel;

	Transform* playerTransform;

	Vector2 scoreTextPosition = {25, 10};
	Text* scoreText;
	int score;

	bool isPlayerAlive;
	float playerRespawnTimer;
	float playerRespawnTime = 4.0f;

	int life;
	Vector2 lifeImagesPosition = {20, 50};
	std::vector<Image*> lifeImages;

	Canvas* canvas;

	float nextLevelTimer;
	float timeToLoadLevel = 3.0f;
	bool loadNextLevel = false;

	float nextUFOTimer;
	int UFOsalive;

	bool isPaused = false;

	PauseMenu* pauseMenu;
	GameOverMenu* gameOverMenu;

	std::map<int, Predef> idObjectDefinitions;
	std::map<Predef, std::vector<int>> spawnedObjects;
	std::map<int, GameObject*> idGameObjects;
};