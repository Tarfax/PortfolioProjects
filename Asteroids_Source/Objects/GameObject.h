#pragma once

#include "IObject.h"

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <Component/Core/IComponent.h>
#include <FactorySystem/Predef.h>

class BoxCollider2D;
class Transform;
class Renderer;
class GameObject: public IObject {

public:
	GameObject();
	void OnInit();
	void OnEnable() override;
	void OnStart() override {}
	void OnUpdate(float deltaTime) override;
	void OnDraw(SDL_Renderer* renderer) override;
	void OnDisable() override;
	void OnDestroy() override;

	void OnCollision(BoxCollider2D* other) { 
		for (int i = 0; i < components.size(); i++) 	{
			components[i]->Collision(other);
		}
	}

	void SetActive(bool beActive);
	bool IsActive();

public: //Components

	template <typename T, typename = std::enable_if_t<std::is_base_of<IComponent, T>::value>>
	inline T* AddComponent() {
		T* component = new T(this);

		component->Init();
		components.push_back(component);
		return component;
	}

	template <typename T, typename = std::enable_if_t<std::is_base_of<IComponent, T>::value>>
	inline T* GetComponent() {
		for (IComponent* component : components) {
			if (typeid(T) == typeid(*component)) {
				return (T*)component;
			}
		}
		return nullptr;
	}

	inline std::string ToString() { return name + " " + std::to_string(id); }

	Renderer* renderer;
	BoxCollider2D* collider;
	Transform* transform;
	std::string name;
	const int id;

private:
	std::vector<IComponent*> components;

public: //Static

	static void Init();
	static void Enable();
	static void Update(float deltaTime);
	static void Draw(SDL_Renderer* renderer);
	static void Disable();
	static void Destroy(GameObject* gameObject, Predef predef = Predef::Unknown);
	static void CleanUp();
	static void ClearOut(std::map<int, GameObject*>::iterator it);

private:

	static int nextId;
	static std::map<int, GameObject*> gameObjects;
	static std::map<int, GameObject*> gameObjectsDisabled;
	static std::map<int, GameObject*> gameObjectsToDestroy;

	static std::set<int> gameObjectsToEnable;
	static std::set<int> gameObjectsToDisable;

	static std::map<int, GameObject*> gameObjectsToEnableMap;
	static std::map<int, GameObject*> gameObjectsToDisableMap;
	static std::map<int, GameObject*> activeGameObjects;

	static std::map<int, GameObject*> gameObjectsToInit;

};