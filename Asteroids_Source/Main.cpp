#include "Core/Engine.h"

int main(int argc, char* args[]) {
	Engine engine;

	if (engine.Init() == true) {
		engine.Run();
		engine.Quit();
	}

	return 0;
}