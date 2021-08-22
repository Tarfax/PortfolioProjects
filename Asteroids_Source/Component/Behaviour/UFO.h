#pragma once
#include "Component/Behaviour/Behaviour.h"
#include <FactorySystem/FactoryObject.h>

struct ObjectData;
class UFO : public Behaviour, public FactoryObject {

public:
	UFO(GameObject* gameObject) : Behaviour(gameObject) {}

protected:
	void OnInit() override;
	void OnUpdate(float deltaTime) override;
	void OnEnable() override;
	void OnCollision(BoxCollider2D* collider) override;
	void OnDisable() override;
	void OnDestroy() override;
	void OnSetData(ObjectData* data) override;
private:

	Vector2 direction{ 0, 0 };
	float speed{};
	float position{};
	float magnitude{};
	float frequency{};
	float projectileSpeed{};

	void Fire();

	int counter{};
	int framesToNextShot{};

	bool deathByPlayer = false;

	float ufoSoundTimer;
	float ufoSoundTime = 0.5f;
};