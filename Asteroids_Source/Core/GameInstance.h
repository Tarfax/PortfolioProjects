#pragma once
#include <FSM/GameState.h>

class GameState;
class GameInstance {
public:
	GameInstance();
	void Init();
	void Update(float deltaTime);
	void Destroy();

	template <typename T, typename = std::enable_if_t<std::is_base_of<GameState, T>::value>>
	inline void ChangeToState() {
		switchToNewState = true;
		newState = new T();
	}

	void SetTimeScale(float scale);
	void SetPaused(bool pauseState);

private: 
	void SwitchState();

	bool switchToNewState;
	GameState* state;
	GameState* newState;

	float timeScale = 1;
	bool isPaused;
};

