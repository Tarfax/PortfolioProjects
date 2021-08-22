#include "Behaviour.h"

Behaviour::Behaviour(GameObject* gameObject) :IComponent(gameObject) {
	transform = gameObject->GetComponent<Transform>();
}
