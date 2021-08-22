#pragma once

#include <Structs/Vector2.h>
#include <string>
#include <Structs/Sprite.h>
#include <UI/UIElement.h>

class Image: public UIElement {
public:
	Image(std::string id) {
		sprite = Sprite::CreateSprite(id);
	}

	void Init() override {
		SetSize();
	}

	void Update(float deltaTime) override{ }
	void Destroy() override { }

};