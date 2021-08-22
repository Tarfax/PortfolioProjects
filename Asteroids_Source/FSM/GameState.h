#pragma once

#include <string>
#include <iostream>
#include <SoundSystem/SoundCoordinator.h>
#include <UI/UIFactory.h>
#include <Component/Core/Canvas.h>
#include <Core/Input.h>

class GameInstance;

class GameState {

public:
	virtual ~GameState() { }
	inline void Enter(GameInstance* gameInstance) {
		this->gameInstance = gameInstance;
		OnEnter();
	}
	inline void Update(float deltaTime) {
		OnUpdate(deltaTime);
	}
	inline void Exit() {
		OnExit();
	}

	virtual std::string ToString() { return "Undefined state"; }

protected:
	virtual void OnEnter() = 0;
	virtual void OnUpdate(float deltaTime) = 0;
	virtual void OnExit() = 0;

	GameInstance* gameInstance;
};