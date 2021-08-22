#include "Component/Core/Canvas.h"

void Canvas::OnUpdate(float deltaTime) {
	for (int i = 0; i < elements.size(); i++) {
		elements[i]->Update(deltaTime);
	}
}

void Canvas::OnDestroy() {
	for (int i = 0; i < elements.size(); i++) 	{
		elements[i]->Destroy();
	}
	for (int i = 0; i < elements.size(); i++) {
		delete elements[i];
	}
	elements.clear();
}

void Canvas::AddUIElement(UIElement* element) {
	elements.push_back(element);
}

void Canvas::RemoveUIElement(UIElement* element) {
	for (int i = 0; i < elements.size(); i++) 	{
		if (elements[i] == element) {
			elements.erase(elements.begin() + i);
		}
	}
}


void Canvas::OnDraw(SDL_Renderer* renderer) {

	for (int i = 0; i < elements.size(); i++) {
		UIElement& element = *elements[i];
		SDL_Texture* texture = element.GetSprite()->Texture;
		SDL_Rect& source = element.GetSprite()->Rect;
		SDL_Rect destination = element.GetPosition();
		double rotation = element.GetRotation();
		Vector2 scale = element.GetScale();
		destination.w *= scale.X;
		destination.h *= scale.Y;
		SDL_RenderCopyEx(renderer, texture, &source, &destination, rotation, nullptr, SDL_FLIP_NONE);
	}

}
