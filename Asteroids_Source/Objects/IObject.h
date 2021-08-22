#pragma once
#include <SDL.h>
#include <Component/Core/IComponent.h>

class IComponent;

class IObject {
public:
	virtual void OnInit() = 0;
	virtual void OnEnable() = 0;
	virtual void OnStart() = 0;
	virtual void OnUpdate(float deltaTime) = 0;

	virtual void OnDraw(SDL_Renderer* renderer) = 0;
	virtual void OnDisable() = 0;
	virtual void OnDestroy() = 0;

};
