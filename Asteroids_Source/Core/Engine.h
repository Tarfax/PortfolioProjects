#pragma once

#include <SDL.h>
#include <Core/EngineData.h>

class Time;
class Input;
class EngineEvent;
class GameInstance;
class Renderer;
class Physics;
class Event;
class EngineCloseEvent;

class Engine {
public:
	Engine();
	
	bool Init();
	void Run();
	void Quit();
	static void QuitProgram();

private:
	void Render();

	void SetupEventSystem();

	void OnEvent(Event& e);
	bool OnWindowClose(EngineCloseEvent& e);

	SDL_Renderer* renderer;
	SDL_Window* window;
	SDL_Surface* screenSurface;
	SDL_Surface* image;

	GameInstance* gameInstance;
	Input* input;
	Physics* physics;

	Time* time;

	static bool isRunning;
};
