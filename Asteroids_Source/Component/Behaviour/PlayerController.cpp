#include "PlayerController.h"
//#include "Core/Input.h"
#include <SDL.h>
//#include <Math/Mathf.h>
#include <Component/Behaviour/PositionWrapper.h>
#include <Component/Behaviour/Projectile.h>
#include <Component/Core/BoxCollider2D.h>
#include <Component/Core/SpriteRenderer.h>
//#include "Structs/Sprite.h"
#include "FactorySystem/Factory.h"
#include "FactorySystem/ObjectDefinitions.h"
#include <EventSystem/ObjectEvent.h>

PlayerController* PlayerController::playerController = nullptr;

void PlayerController::OnInit() {
	playerController = this;

	SpriteRenderer* renderer = gameObject->AddComponent<SpriteRenderer>();
	PositionWrapper* positionWrapper = gameObject->AddComponent<PositionWrapper>();

	Input::AddInputCallback(BindFunction(PlayerController::OnEvent, this), SDL_SCANCODE_W);
	Input::AddInputCallback(BindFunction(PlayerController::OnEvent, this), SDL_SCANCODE_A);
	Input::AddInputCallback(BindFunction(PlayerController::OnEvent, this), SDL_SCANCODE_S);
	Input::AddInputCallback(BindFunction(PlayerController::OnEvent, this), SDL_SCANCODE_D);
	Input::AddInputCallback(BindFunction(PlayerController::OnEvent, this), SDL_SCANCODE_SPACE);
}

void PlayerController::OnEnable() {
	momentum = {0, 0};
	respawnInvulnerable = 3.0f;
	isAlive = true;
	fireRateTimer = fireRate;
}


void PlayerController::OnSetData(ObjectData* data) {
	PlayerData* playerData = dynamic_cast<PlayerData*>(data);
	maxSpeed = playerData->MaxSpeed;
	acceleration = playerData->Acceleration;
	accelFalloffStart = playerData->AccelFalloffStart;
	momentumFalloff = playerData->MomentumFalloff;
	rotationSpeed = playerData->RotationSpeed;
	fireRate = playerData->FireRate;

	SpriteRenderer* renderer = gameObject->GetComponent<SpriteRenderer>();
	renderer->SetSprite(Sprite::CreateSprite(data->TextureIds[0]));

	PositionWrapper* positionWrapper = gameObject->GetComponent<PositionWrapper>();
	positionWrapper->SetTexDimensions(renderer->GetRect());

	BoxCollider2D* collider = gameObject->GetComponent<BoxCollider2D>();
	collider->SetBounds(renderer->GetRect());
	collider->SetLayer(Layer::Player);
}

void PlayerController::OnEvent(Event& e) {
	if (isAlive == true) {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(BindFunction(PlayerController::OnKeyPressedEvent, this));
	}
}

bool PlayerController::OnKeyPressedEvent(KeyPressedEvent& e) {
	if (e.GetKeyCode() == SDL_SCANCODE_W) {
		momentumFalloff = 0.9992f;

		float accelerationFalloff = 
			Mathf::InverseLerp(maxSpeed, accelFalloffStart, momentum.Magnitude());

		Vector2 speedVector = transform->forward * acceleration;
		momentum += speedVector * accelerationFalloff;

		float time = 0.04f;
		static float engineSoundTimer;
		engineSoundTimer -= e.GetDeltaTime();
		if (engineSoundTimer <= 0) {
			SoundCoordinator::PlayEffect("res/SoundFx/engine.wav");
			engineSoundTimer = time;
		}
	}

	if (e.GetKeyCode() == SDL_SCANCODE_A) {
		transform->Rotation() -= (double)(rotationSpeed * e.GetDeltaTime());
	}

	if (e.GetKeyCode() == SDL_SCANCODE_D) {
		transform->Rotation() += (double)(rotationSpeed * e.GetDeltaTime());
	}

	if (e.GetKeyCode() == SDL_SCANCODE_SPACE) {
		if (fireRateTimer <= 0.0f) {
			if (e.GetDeltaTime() != 0) {
				Fire();
				fireRateTimer = fireRate;
			}
		}
	}

	return true;
}

void PlayerController::OnUpdate(float deltaTime) {
	if (respawnInvulnerable > 0.0f) {
		respawnInvulnerable -= deltaTime;
	}

	fireRateTimer -= deltaTime;

	HandleInput(deltaTime);

	momentum *= momentumFalloff;
	transform->Translate(momentum * deltaTime);
	momentumFalloff = 0.9999f;
}


void PlayerController::HandleInput(float deltaTime) {
	if (Input::GetKeyDown(SDL_SCANCODE_H)) {
		transform->Scale().X -= 1 * deltaTime;
	}
}


void PlayerController::Fire() {
	GameObject* gameObject = Factory::Create<Projectile>(Predef::Projectile);

	Transform* projectileTransform = gameObject->GetComponent<Transform>();

	Vector2 position = this->transform->Position();
	float rotation = this->transform->Rotation();

	float x = cos(Mathf::DegToRad(rotation));
	float y = sin(Mathf::DegToRad(rotation));

	int width = this->gameObject->GetComponent<BoxCollider2D>()->GetBounds().w;
	int height = this->gameObject->GetComponent<BoxCollider2D>()->GetBounds().h;

	position.X += (width / 2 * x) + width / 2;
	position.Y += (height / 2 * y) + height / 2;

	projectileTransform->Position() = position;
	gameObject->GetComponent<BoxCollider2D>()->ResetBounds();
	Projectile* projectile = gameObject->GetComponent<Projectile>();
	projectile->SetDirection(this->transform->forward);

	SoundCoordinator::PlayEffect("res/SoundFx/fire4.wav");

	gameObject->SetActive(true);
}

void PlayerController::OnDestroy() {
	Input::RemoveInputCallback(BindFunction(PlayerController::OnEvent, this), SDL_SCANCODE_W);
	Input::RemoveInputCallback(BindFunction(PlayerController::OnEvent, this), SDL_SCANCODE_A);
	Input::RemoveInputCallback(BindFunction(PlayerController::OnEvent, this), SDL_SCANCODE_S);
	Input::RemoveInputCallback(BindFunction(PlayerController::OnEvent, this), SDL_SCANCODE_D);
	Input::RemoveInputCallback(BindFunction(PlayerController::OnEvent, this), SDL_SCANCODE_SPACE);

	PlayerDestroyedEvent e(gameObject, gameObject->GetComponent<BoxCollider2D>(), predefData);
	FireDestroyedEvent(e);
	isAlive = false;
}

void PlayerController::OnCollision(BoxCollider2D* other) {
	if (respawnInvulnerable < 0.0f) {
		Layer collisionLayer = other->GetLayer();
		if (collisionLayer == Layer::UFOProjectile || collisionLayer == Layer::Asteroid) {
			GameObject::Destroy(gameObject, predefData);
		}
	}
}

void PlayerController::OnDisable() {
	PlayerDestroyedEvent e(gameObject, gameObject->GetComponent<BoxCollider2D>(), predefData);
	FireDestroyedEvent(e);
	isAlive = false;
}
