#pragma once
#include <Component/Core/IComponent.h>
#include <Component/Core/Transform.h>
#include <Objects/GameObject.h>

class GameObject;
class Behaviour : public IComponent {

public:
	Behaviour(GameObject* gameObject);
	virtual void OnInit() = 0;
	virtual void OnEnable() { }
	virtual void OnUpdate(float deltaTime) { };
	virtual void OnDisable() { };
	virtual void OnDestroy() { };
protected:
	Transform* transform;
};