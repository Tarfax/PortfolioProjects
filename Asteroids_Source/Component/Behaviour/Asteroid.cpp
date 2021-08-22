#include "Asteroid.h"

#include "Component/Behaviour/PositionWrapper.h"
#include "Component/Core/BoxCollider2D.h"
#include "Component/Core/SpriteRenderer.h"

#include <FactorySystem/PredefinedObject.h>

#include <EventSystem/ObjectEvent.h>


void Asteroid::OnInit() {
	gameObject->AddComponent<SpriteRenderer>();
	gameObject->AddComponent<PositionWrapper>();
}


void Asteroid::OnSetData(ObjectData* data) {
	AsteroidData* asteroidData = dynamic_cast<AsteroidData*>(data);
	transform->Scale() = asteroidData->Scale;

	speed = Mathf::RandomFloat(asteroidData->MinSpeed, asteroidData->MaxSpeed);

	level = asteroidData->Level;

	SpriteRenderer* spriteRenderer = gameObject->GetComponent<SpriteRenderer>();
	spriteRenderer->SetSprite(Sprite::CreateSprite(data->TextureIds[0]));

	PositionWrapper* positionWrapper = gameObject->GetComponent<PositionWrapper>();
	positionWrapper->SetTexDimensions(spriteRenderer->GetRect());

	BoxCollider2D* collider = gameObject->GetComponent<BoxCollider2D>();
	collider->SetBounds(spriteRenderer->GetRect());
	collider->SetLayer(Layer::Asteroid);

	rotationSpeed = Mathf::RandomFloat() * 10.0f;
	direction.X = Mathf::RandomFloat(-1, 1);
	direction.Y = Mathf::RandomFloat(-1, 1);
	direction.Normalize();

	transform->Rotation() = Mathf::RandomFloat(0, 360);
}

void Asteroid::OnUpdate(float deltaTime)
{
	transform->Translate(Vector2((speed * deltaTime) * direction.X, (speed * deltaTime) * direction.Y));
	transform->Rotation() += (double)rotationSpeed * (double)deltaTime;

	if (Input::GetKeyDown(SDL_SCANCODE_K)) {
		std::cout << "Kill?" << std::endl;
	}
}

void Asteroid::OnDisable() {
	FireAsteroidDestroyedEvent();
}

void Asteroid::OnDestroy() {
	FireAsteroidDestroyedEvent();
}

void Asteroid::OnCollision(BoxCollider2D* other) {
	if (other->GetLayer() == Layer::Projectile) {
		GameObject::Destroy(gameObject, predefData);
	}
}

void Asteroid::FireAsteroidDestroyedEvent() {
	AsteroidDestroyedEvent event = {gameObject, gameObject->GetComponent<BoxCollider2D>(), level, predefData};
	FireDestroyedEvent(event);
}