#include "Core/Input.h"
#include <string>
#include <Core/Engine.h>
#include <EventSystem/EngineEvent.h>

Input* Input::instance = nullptr;

Input* Input::Init() {
	instance = (instance != nullptr) ? instance : new Input();
	instance->keyStates = SDL_GetKeyboardState(nullptr);
	return instance;
}

void Input::Listen(float deltaTime) {
	this->deltaTime = deltaTime;
	Reset();

	while (SDL_PollEvent(&e) == true) {
		// switch to allow further implementation of inputs.
		switch (e.type) {

			case SDL_KEYDOWN:
				KeyDown();
				break;
			case SDL_KEYUP:
				KeyUp();
				break;
			case SDL_QUIT:
				EngineCloseEvent event { };
				FireEvent(event);
				break;
		}

	}

	SendKeyCallbacks();
}


void Input::Reset() {
	std::unordered_map<SDL_Scancode, bool>::iterator it;
	for (it = wasKeyUp.begin(); it != wasKeyUp.end(); it++) {
		wasKeyUp[it->first] = false;
	}
}

void Input::SendKeyCallbacks() {
	//Checking key down
	std::unordered_map<SDL_Scancode, std::vector<CallbackData>>::iterator it;
	for (it = inputCallbacks.begin(); it != inputCallbacks.end(); it++) {
		SDL_Scancode key = it->first;

		if (keyStates[key] == 1) {
			if (keyStateFrameCount.count(key) != 1) {
				keyStateFrameCount[key] = 0;
			}


			KeyPressedEvent event {key, instance->keyStateFrameCount[key], deltaTime * timeScale};

			for (int i = 0; i < it->second.size(); i++) {
				FireEvent(event, it->second[i]);
			}

			keyStateFrameCount[key]++;
			keyWasCounted = true;
			//std::cout << "key " << key << " was pressed\n ";
		}
	}

	//Checking key up
	std::unordered_map<SDL_Scancode, int>::iterator it2;
	for (it2 = keyStateFrameCount.begin(); it2 != keyStateFrameCount.end(); it2++) {
		if (keyStates[it2->first] == 0 && it2->second > 0) {

			KeyReleasedEvent event {it2->first};

			for (int i = 0; i < inputCallbacks[it2->first].size(); i++) {
				FireEvent(event, inputCallbacks[it2->first][i]);
			}

			wasKeyUp[it2->first] = true;
			it2->second = 0;
			//std::cout << it2->first << " was released" << std::endl;
		}
	}
}

void Input::FireEvent(Event& event, CallbackData data) {
	data.EventCallback(event);
}

void Input::FireEvent(Event& event) {
	for (int i = 0; i < callbacks.size(); i++) {
		CallbackData data = callbacks[i];
		data.EventCallback(event);
	}
}

bool Input::GetKeyDown(SDL_Scancode key) {
	if (instance->keyStates[key] == 1) {
		//std::cout << key << " was pressed " << instance->keyStateFrameCount[key] << std::endl;

		if (instance->keyWasCounted == false) {
			instance->keyStateFrameCount[key]++;
		}

		if (instance->keyStateFrameCount[key] > 1) {
			return false;
		}

		return true;
	}
	return false;
}

bool Input::GetKeyUp(SDL_Scancode key) {
	if (instance->wasKeyUp.count(key) == 1) {
		return instance->wasKeyUp[key] == true;
	}
	else {
		instance->wasKeyUp[key] = true;
		return true;
	}
	return false;
}

bool Input::GetKey(SDL_Scancode key) {
	return instance->keyStateFrameCount[key] > 1;
}


void Input::KeyDown() {
	keyStates = SDL_GetKeyboardState(nullptr);
}

void Input::SetTimeScale(float scale){
	instance->timeScale = scale;
}

void Input::KeyUp() {
	keyStates = SDL_GetKeyboardState(nullptr);
}

void Input::AddCallback(const EventCallbackFunc& callback) {
	callbacks.emplace_back(callback);
}

void Input::RemoveCallback(const EventCallbackFunc& callback) {
	for (int i = 0; i < callbacks.size(); i++) {
		CallbackData cd = callbacks[i];
		if (cd.EventCallback.target_type().hash_code() == callback.target_type().hash_code()) {
			//std::cout << "onEnterPressed found" << std::endl;
			callbacks.erase(callbacks.begin() + i);
			return;
		}
	}
}

/* --Can't get this to work? */

//void Input::AddInputCallback(const EventCallbackFunc& onEnterPressed, SDL_Scancode keyCode) {
//	instance->inputCallbacks[keyCode].emplace_back(onEnterPressed);
//}
//
//void Input::RemoveInputCallback(const EventCallbackFunc& onEnterPressed, SDL_Scancode keyCode) {
//	for (int i = 0; i < instance->inputCallbacks[keyCode].size(); i++) {
//		CallbackData cd = instance->inputCallbacks[keyCode][i];
//		if (cd.EventCallback.target_type().hash_code() == onEnterPressed.target_type().hash_code()) {
//			//std::cout << "onEnterPressed found" << std::endl;
//			instance->inputCallbacks[keyCode].erase(instance->inputCallbacks[keyCode].begin() + i);
//			return;
//		}
//	}
//}
