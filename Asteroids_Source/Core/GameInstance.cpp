#include "GameInstance.h"
#include <FSM/MainMenuState.h>

GameInstance::GameInstance() { }

void GameInstance::Init() {
	ChangeToState<MainMenuState>();
}

void GameInstance::Update(float deltaTime) {
	if (switchToNewState == true) {
		SwitchState();
		switchToNewState = false;
	}

	state->Update(deltaTime);
	if (isPaused == false) {
		GameObject::Update(deltaTime);
	}
}

void GameInstance::Destroy() {
	if (state != nullptr) {
		state->Exit();
		delete state;
		state = nullptr;
	}
}

void GameInstance::SwitchState() {
	if (state != nullptr) {
		state->Exit();
	}
	delete state;
	state = newState;
	state->Enter(this);
	newState = nullptr;
}

void GameInstance::SetTimeScale(float scale) {
	Input::SetTimeScale(scale);
	timeScale = scale;
}

void GameInstance::SetPaused(bool paused) {
	isPaused = paused;
}