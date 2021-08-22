#pragma once
#include "Component/Behaviour/Behaviour.h"
#include "FactorySystem/FactoryObject.h"

class ObjectData;

class Asteroid: public Behaviour, public FactoryObject {

public:
	Asteroid(GameObject* gameObject): Behaviour(gameObject) { gameObject->name = "Asteroid"; }

protected:
	void OnInit() override;
	void OnUpdate(float deltaTime) override;
	void OnDisable() override;
	void OnDestroy() override;

	void OnSetData(ObjectData* data) override;

private:
	void OnCollision(BoxCollider2D* other) override;

	void FireAsteroidDestroyedEvent();

	float speed { };
	float rotationSpeed { };
	Vector2 direction { };
	int level;

};