#include "Projectile.h"

#include <Component/Core/BoxCollider2D.h>
#include <Component/Core/SpriteRenderer.h>
#include "Component/Behaviour/PositionWrapper.h"

Projectile::~Projectile() { }

void Projectile::OnInit() {
	gameObject->AddComponent<SpriteRenderer>();
	gameObject->AddComponent<PositionWrapper>();
}

void Projectile::OnSetData(ObjectData* data) {

	ProjectileData* projectileData = dynamic_cast<ProjectileData*>(data);
	speed = projectileData->Speed;
	lifeTime = projectileData->LifeTime;
	lifeTimeCounter = lifeTime;
	rotationSpeed = projectileData->RotationSpeed;

	SpriteRenderer* renderer = gameObject->GetComponent<SpriteRenderer>();
	renderer->SetSprite(Sprite::CreateSprite(data->TextureIds[0]));

	PositionWrapper* positionWrapper = gameObject->GetComponent<PositionWrapper>();
	positionWrapper->SetTexDimensions(renderer->GetRect());

	BoxCollider2D* collider = gameObject->GetComponent<BoxCollider2D>();
	collider->SetBounds(renderer->GetRect());
	collider->SetLayer(Layer::Projectile);
	Vector2 scale = 1 * projectileData->Scale;
	transform->Scale() = scale;
}

void Projectile::OnUpdate(float deltaTime) {
	transform->Translate(Vector2((speed * deltaTime) * direction.X, (speed * deltaTime) * direction.Y));

	if (rotationSpeed > 0) {
		transform->Rotation() += rotationSpeed * deltaTime;
	}

	lifeTimeCounter -= deltaTime;
	if (lifeTimeCounter < 0.0f) {
		GameObject::Destroy(gameObject, Predef::Projectile);
		lifeTimeCounter = lifeTime;
	}
}

void Projectile::OnCollision(BoxCollider2D* other) {
	if (other->GetLayer() == Layer::Asteroid) {
		GameObject::Destroy(gameObject, predefData);
	}
}

void Projectile::SetDirection(Vector2 direction) {
	this->direction = direction;
}
