#pragma once
#include "Component/Behaviour/Behaviour.h"
#include <SDL.h>

class PositionWrapper : public Behaviour {
public:
	PositionWrapper(GameObject* gameObject): Behaviour(gameObject) { }

	void SetTexDimensions(SDL_Rect rect);

protected:
	void OnInit() override { }
	void OnUpdate(float deltaTime) override;

private:
	Vector2 texDimensions { 0, 0 };
};