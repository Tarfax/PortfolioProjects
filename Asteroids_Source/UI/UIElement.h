#pragma once
#include <SDL.h>
#include <Structs/Sprite.h>
#include <iostream>

class UIElement {

public:
	virtual ~UIElement() {
		//std::cout << "UI Element " << sprite->TextureId << " gone " << std::endl;
	}

	virtual void Init() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Destroy() = 0;

	void SetPosition(Vector2 position) { this->position.x = position.X; this->position.y = position.Y; }
	void SetPosition(int x, int y) { this->position.x = x; this->position.y = y; }
	void SetSize(Vector2 size) { this->position.w = size.X; this->position.h = size.Y; }

	/// <summary>
	/// Sets the size to the sprite's size.
	/// </summary>
	void SetSize() { this->position.w = sprite->Size.X; this->position.h = sprite->Size.Y; }
	void SetRotation(double rotation) { this->rotation = rotation; }
	void SetScale(Vector2 scale) { this->scale = scale; }
	void SetScale(float scale) { this->scale.X = scale, this->scale.Y = scale; }

	SDL_Rect GetPosition() const { return position; }
	Vector2 GetScale() const { return scale; }
	float GetRotation() const { return rotation; }
	Sprite* GetSprite() const { return sprite; }

protected:
	SDL_Rect position;
	Sprite* sprite;
	double rotation;
	Vector2 scale {1, 1};

};