#pragma once
#include "Component/Behaviour/Behaviour.h"
#include "FactorySystem/FactoryObject.h"

struct ObjectData;
class Projectile : public Behaviour, public FactoryObject { 
public:
	Projectile(GameObject* gameObject): Behaviour(gameObject) { gameObject->name = "Projectile"; }
	
	~Projectile();

	void SetDirection(Vector2 direction);

protected:
	void OnInit() override;
	void OnUpdate(float deltaTime) override;
	void OnSetData(ObjectData* data) override;
	void OnCollision(BoxCollider2D* other);

private:
	Vector2 direction;
	float speed;
	float lifeTime;
	float lifeTimeCounter;
	float rotationSpeed;
};