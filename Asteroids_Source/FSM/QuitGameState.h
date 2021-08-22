#pragma once
#include <FSM/GameState.h>

class QuitGameState : public GameState {

	void OnEnter() override;
	void OnUpdate(float deltaTime) override;
	void OnExit() override;

};