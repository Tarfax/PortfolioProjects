#pragma once

#include <Structs/Vector2.h>
#include <vector>
#include <Structs/Sprite.h>
#include <Core/Input.h>
#include <UI/UIElement.h>
#include <UI/Text.h>
#include <iostream>

enum class ButtonState { Normal = 0, Selected = 1 };

class Button: public UIElement {

public:
	Button(std::string normalButton, std::string selectedButton, std::function<void(KeyPressedEvent&)> callback) {
		//std::cout << "Button object created: " << normalButton << std::endl;

		normalSprite = Sprite::CreateSprite(normalButton);
		selectedSprite = Sprite::CreateSprite(selectedButton);
		onEnterPressed = callback;
		sprite = normalSprite;
	}

	//~Button() {
	//	std::cout << "Button object ~: " << normalSprite->TextureId << std::endl;
	//}

	void ListenForInput(SDL_Scancode key);

	void Init() override;
	void Destroy() override;
	void Update(float deltaTime) override;

	void SetState(ButtonState newState);

	void SetText(Text* text);
	void SetPositionAndText(Vector2 position);


private:
	std::vector<SDL_Scancode> keyCodes;

	void OnEvent(Event& event);
	bool OnKeyPressedEvent(KeyPressedEvent& e);

	int leftMargin = 25;

	Sprite* normalSprite;
	Sprite* selectedSprite;

	Text* text;

	ButtonState buttonState;
	bool isReleased;
	std::function<void(KeyPressedEvent&)> onEnterPressed;

};