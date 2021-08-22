#pragma once

#include <Component/Core/Renderer.h>
#include <UI/UIElement.h>

class Canvas: public Renderer {

public:
	Canvas(GameObject* gameObject): Renderer(gameObject) { gameObject->name = "Canvas"; }

	void AddUIElement(UIElement* element);
	void RemoveUIElement(UIElement* element);

protected:
	void OnDraw(SDL_Renderer* renderer) override;
	void OnUpdate(float deltaTime) override;
	void OnDestroy() override;

private:
	std::vector<UIElement*> elements;
};