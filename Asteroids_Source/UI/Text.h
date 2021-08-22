#pragma once

#include <UI/UIElement.h>
#include <Structs/Sprite.h>
#include <SDL.h>
#include <TextureCoordinator/TextureCoordinator.h>
#include <iostream>

class Text: public UIElement {

public:
	Text(std::string text, SDL_Color color, int size) {
		msg = text;
		//std::cout << "Text object created: " << text << std::endl;
		sprite = TextureCoordinator::GetSpriteFromText(text, "res/Fonts/novamono.ttf", color, size);
	}

	//~Text() {
	//	std::cout << "Text object ~: " << msg << std::endl;
	//}

	inline void Init() override {
		SetSize();
	}

	inline void Update(float deltaTime) override { }

	inline void Destroy() override {
		sprite->Destroy();
	}

	std::string msg;

};
