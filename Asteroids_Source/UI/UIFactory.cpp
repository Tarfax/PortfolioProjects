#include "UIFactory.h"

Text* UIFactory::CreateText(std::string msg, SDL_Color color, int size) {
	Text* text = new Text(msg, color, size);
	text->Init();
	return text;
}

Button* UIFactory::CreateButton(std::string normal, std::string selected, Vector2 position, Text* text, ButtonCallback cb) {
	Button* button = new Button(normal, selected, cb);
	button->Init();
	button->SetPosition(position);
	button->SetText(text);
	return button;
}