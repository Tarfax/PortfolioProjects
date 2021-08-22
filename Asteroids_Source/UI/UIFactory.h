#pragma once
#include <UI/Button.h>
#include <UI/Image.h>
#include <UI/Text.h>

class UIFactory {
	using ButtonCallback = std::function<void(KeyPressedEvent&)>;

public:
	static Button* CreateButton(std::string normal, std::string selected, Vector2 position, Text* text, ButtonCallback cb);
	static Text* CreateText(std::string msg, SDL_Color color, int size);
};