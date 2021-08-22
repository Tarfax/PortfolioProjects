#pragma once

#include <chrono>

class Time {

public:
	void StartTick();
	void EndTick();
	float GetDeltaTime() const{ return deltaTime; }

	static float CurrentTime() {
		std::chrono::steady_clock::time_point start = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> currentTime = start.time_since_epoch();
		return currentTime.count();
	}

private:
	float deltaTime;
	std::chrono::steady_clock::time_point start;
	std::chrono::steady_clock::time_point end;
	std::chrono::duration<float> difference;
};
