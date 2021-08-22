#include "Button.h"
#include <string>
#include <iostream>

void Button::Init() {
	//Input::AddInputCallback(BindFunction(Button::OnEvent, this), SDL_SCANCODE_SPACE);
	SetSize();
}

void Button::Destroy() {
	//Input::RemoveInputCallback(BindFunction(Button::OnEvent, this), SDL_SCANCODE_SPACE);

	for (int i = 0; i < keyCodes.size(); i++) {
		Input::RemoveInputCallback(BindFunction(Button::OnEvent, this), keyCodes[i]);

	}
	normalSprite->Destroy();
	selectedSprite->Destroy();
}

void Button::Update(float deltaTime) {

}


void Button::SetState(ButtonState newState) {
	buttonState = newState;
	if (buttonState == ButtonState::Normal) {
		sprite = normalSprite;
	}
	else {
		sprite = selectedSprite;
	}
}

void Button::SetText(Text* text) {
	if (text != nullptr) {
		SDL_Rect size = text->GetPosition();
		text->SetPosition(Vector2(position.x + leftMargin, position.y + (position.h / 2) - (size.h / 2)));
		this->text = text;
	}
}

void Button::SetPositionAndText(Vector2 newPosition) {
	SetPosition(newPosition);
	SDL_Rect size = text->GetPosition();
	text->SetPosition(Vector2(position.x + leftMargin, position.y + (position.h / 2) - (size.h / 2)));
}

void Button::OnEvent(Event& event) {
	EventDispatcher dispatcher(event);// = EventDispatcher(event);
	dispatcher.Dispatch<KeyPressedEvent>(BindFunction(Button::OnKeyPressedEvent, this));
}

bool Button::OnKeyPressedEvent(KeyPressedEvent& e) {
	if (/*e.GetKeyCode() == SDL_SCANCODE_SPACE && */e.GetRepeatCount() == 0 && buttonState == ButtonState::Selected) {
		onEnterPressed(e);
	}
	return true;
}

void Button::ListenForInput(SDL_Scancode key) {
	keyCodes.push_back(key);
	Input::AddInputCallback(BindFunction(Button::OnEvent, this), key);
}
