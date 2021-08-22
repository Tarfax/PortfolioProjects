#include "UFO.h"
#include "PlayerController.h"
#include "FactorySystem/Factory.h"
#include "Component/Behaviour/Projectile.h"
#include "Component/Behaviour/PositionWrapper.h"
#include "Component/Core/BoxCollider2D.h"
#include "Component/Core/SpriteRenderer.h"
#include <EventSystem/ObjectEvent.h>

void UFO::OnInit()
{
	gameObject->AddComponent<SpriteRenderer>();
	gameObject->AddComponent<PositionWrapper>();

	counter = 0;
	framesToNextShot = Mathf::RandomFloat(3000, 8000);

}

void UFO::OnEnable() {
	ufoSoundTimer = 0.0f;
}

void UFO::OnSetData(ObjectData* data)
{
	UFOData* ufoData = dynamic_cast<UFOData*>(data);
	speed = ufoData->Speed;
	projectileSpeed = ufoData->ProjectileSpeed;

	SpriteRenderer* spriteRenderer = gameObject->GetComponent<SpriteRenderer>();
	spriteRenderer->SetSprite(Sprite::CreateSprite(data->TextureIds[0]));

	PositionWrapper* positionWrapper = gameObject->GetComponent<PositionWrapper>();
	positionWrapper->SetTexDimensions(spriteRenderer->GetRect());

	BoxCollider2D* collider = gameObject->GetComponent<BoxCollider2D>();
	collider->SetBounds(spriteRenderer->GetRect());
	collider->SetLayer(Layer::UFO);

	float dir = Mathf::RandomFloat(-1, 1);
	direction.X = dir / fabsf(dir);

	position = Mathf::RandomFloat(ufoData->PositionRangeMin, ufoData->PositionRangeMax);
	magnitude = Mathf::RandomFloat(ufoData->MagnitudeRangeMin, ufoData->MagnitudeRangeMax);
	frequency = Mathf::RandomFloat(ufoData->FrequencyRangeMin, ufoData->FrequencyRangeMax);
}

void UFO::Fire()
{
	Vector2 test = PlayerController::
		playerController->gameObject->GetComponent<Transform>()->Position();

	GameObject* object = Factory::Create<Projectile>(Predef::UFOProjectile);

	Projectile* projectile = object->GetComponent<Projectile>();
	Vector2 direction = Vector2::Direction(test, transform->Position());
	projectile->SetDirection(direction *= projectileSpeed);

	Transform* projectileTransform = object->GetComponent<Transform>();

	Vector2 position = transform->Position();

	float width = this->gameObject->GetComponent<BoxCollider2D>()->GetBounds().w;
	float height = this->gameObject->GetComponent<BoxCollider2D>()->GetBounds().h;

	position.X += width * 0.5f;
	position.Y += height * 0.7f;

	projectileTransform->Position() = position;

	BoxCollider2D* collider = object->GetComponent<BoxCollider2D>();
	collider->SetLayer(Layer::UFOProjectile);

	SoundCoordinator::PlayEffect("res/SoundFx/UFOFire.wav");
}

void UFO::OnUpdate(float deltaTime)
{
	transform->Position().Y = (sinf(transform->Position().X * frequency) * magnitude) + position;
	transform->Translate(Vector2((speed * deltaTime) * direction.X, 0.0f));

	if (counter >= framesToNextShot) {
		Fire();
		counter = 0;
		framesToNextShot = Mathf::RandomFloat(3000, 8000);
	}
	counter++;

	ufoSoundTimer -= deltaTime;
	if (ufoSoundTimer <= 0.0f) {
		SoundCoordinator::PlayEffect("res/SoundFx/UFOAlive.wav");
		ufoSoundTimer = ufoSoundTime;
	}
}

void UFO::OnCollision(BoxCollider2D* collider) {
	deathByPlayer = false;
	if (collider->GetLayer() == Layer::Asteroid) {
		GameObject::Destroy(gameObject, Predef::UFO);
	}
	else if (collider->GetLayer() == Layer::Projectile) {
		GameObject::Destroy(gameObject, Predef::UFO);
		deathByPlayer = true;
		SoundCoordinator::PlayEffect("res/SoundFx/UFODies.wav");
	}
}

void UFO::OnDisable() {
	UFODestroyedEvent e(gameObject, gameObject->GetComponent<BoxCollider2D>(), predefData, deathByPlayer);
	FireDestroyedEvent(e);
}

void UFO::OnDestroy() {
	UFODestroyedEvent e(gameObject, gameObject->GetComponent<BoxCollider2D>(), predefData, deathByPlayer);
	FireDestroyedEvent(e);
}
