#include "BoxCollider2D.h"
#include "Objects/GameObject.h"
#include <EventSystem/ObjectEvent.h>

bool BoxCollider2D::renderCollider;
int BoxCollider2D::nextId;

BoxCollider2D::BoxCollider2D(GameObject* gameObject)
	: IComponent(gameObject),
	id(nextId++), bounds({0, 0, 0, 0}), layer(Layer::Nothing),
	originalBounds({0, 0, 0, 0})
{
}

void BoxCollider2D::OnInit() {
	transform = gameObject->GetComponent<Transform>();
}

void BoxCollider2D::OnEnable() {
	ObjectSpawnedEvent e(gameObject, gameObject->GetComponent<BoxCollider2D>());
	FireSpawnedEvent(e);
}

void BoxCollider2D::OnUpdate(float deltaTime) {
	if (Input::GetKeyDown(SDL_SCANCODE_B) == true) {
		renderCollider = !renderCollider;
	}

	bounds = {(int)transform->X(), (int)transform->Y(), (int)(originalBounds.w * transform->Scale().X), (int)(originalBounds.h * transform->Scale().Y)};
}

void BoxCollider2D::OnDisable() {
	ObjectDestroyedEvent e(gameObject, gameObject->GetComponent<BoxCollider2D>());
	FireDestroyedEvent(e);
}

void BoxCollider2D::OnDestroy() {
	ObjectDestroyedEvent e(gameObject, gameObject->GetComponent<BoxCollider2D>());
	FireDestroyedEvent(e);
}

void BoxCollider2D::Draw(SDL_Renderer* renderer, Transform* transform) {
	if (renderCollider == true) {
		SDL_SetRenderDrawColor(renderer, 0, 250, 230, 255);
		SDL_RenderDrawRect(renderer, &bounds);
	}
}

void BoxCollider2D::SetBounds(int x, int y, int h, int w) {
	SetBounds(SDL_Rect {x, y, h, w});
}

void BoxCollider2D::SetBounds(SDL_Rect rect) {
	this->originalBounds = rect;
}

void BoxCollider2D::SetLayer(Layer layer) {
	this->layer = layer;
}

bool BoxCollider2D::IsColliding(BoxCollider2D* other) {
	bool isOverlapX = (bounds.x < other->bounds.x + other->bounds.w) && (bounds.x + bounds.w > other->bounds.x);
	bool isOverlapY = (bounds.y < other->bounds.y + other->bounds.h) && (bounds.y + bounds.h > other->bounds.y);

	return isOverlapX && isOverlapY;
}
