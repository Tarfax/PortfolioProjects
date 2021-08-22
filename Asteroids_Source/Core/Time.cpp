#include "Time.h"
#include <SDL.h>

void Time::StartTick() {
	//Maybe use SDL_GetTicks() instead?
	// SDL_GetTicks();
	start = std::chrono::high_resolution_clock::now();
}

void Time::EndTick() {
	end = std::chrono::high_resolution_clock::now();
	difference = end - start;
	deltaTime = difference.count();
}