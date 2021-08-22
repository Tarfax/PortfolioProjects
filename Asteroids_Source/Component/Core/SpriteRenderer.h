#pragma once

#include <Component/Core/Renderer.h>
class SDL_Renderer;
struct SDL_Rect;

class SpriteRenderer: public Renderer {

public:
	SpriteRenderer(GameObject* gameObject) : Renderer(gameObject) { }
	void SetSprite(Sprite* sprite);
	SDL_Rect GetRect();

protected:
	void OnDestroy() override;
	void OnDraw(SDL_Renderer* renderer) override;

private:
	Sprite* sprite = nullptr;

};