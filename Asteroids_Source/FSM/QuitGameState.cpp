#include "QuitGameState.h"
#include <Core/Engine.h>
void QuitGameState::OnEnter() {
	Engine::QuitProgram();
}

void QuitGameState::OnUpdate(float deltaTime) {}

void QuitGameState::OnExit() { }
