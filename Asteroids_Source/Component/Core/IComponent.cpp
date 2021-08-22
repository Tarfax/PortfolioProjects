#include "IComponent.h"
class BoxCollider2D;
std::vector<IComponent::CallbackData> IComponent::onObjectDestroyed;
std::vector<IComponent::CallbackData> IComponent::onObjectSpawned;

void IComponent::Init() {
	OnInit();
}

void IComponent::Enable() {
	OnEnable();
}

void IComponent::Update(float deltaTime) {
	OnUpdate(deltaTime);
}

void IComponent::Disable() {
	OnDisable();

}

void IComponent::Destroy() {
	OnDestroy();
}

void IComponent::Collision(BoxCollider2D* other) {
	OnCollision(other);
}