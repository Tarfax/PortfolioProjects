#include "GameObject.h"
#include <FactorySystem/Predef.h>
#include <FactorySystem/Factory.h>
#include <Component/Core/Renderer.h>
#include <Component/Core/BoxCollider2D.h>
#include <Component/Core/Transform.h>
#include <FactorySystem/Predef.h>

int GameObject::nextId = 0;
std::map<int, GameObject*> GameObject::gameObjects;
std::map<int, GameObject*> GameObject::gameObjectsToInit;
std::map<int, GameObject*> GameObject::gameObjectsToDestroy;

std::map<int, GameObject*> GameObject::gameObjectsDisabled;
std::set<int> GameObject::gameObjectsToEnable;
std::set<int> GameObject::gameObjectsToDisable;

std::map<int, GameObject*> GameObject::gameObjectsToEnableMap;
std::map<int, GameObject*> GameObject::gameObjectsToDisableMap;
std::map<int, GameObject*> GameObject::activeGameObjects;

GameObject::GameObject(): id(nextId++) {
	transform = AddComponent<Transform>();
	collider = AddComponent<BoxCollider2D>();;
	gameObjects[id] = this;
	gameObjectsToInit[id] = this;
}

void GameObject::OnInit() {
	SetActive(true);
}

void GameObject::OnEnable() {
	for (IComponent* component : components) {
		component->Enable();
	}
}

void GameObject::OnUpdate(float deltaTime) {
	for (IComponent* c : components) {
		c->Update(deltaTime);
	}
}

void GameObject::OnDraw(SDL_Renderer* renderer) {
	if (collider != nullptr) {
		collider->Draw(renderer, transform);
	}
}

void GameObject::OnDisable() {
	for (IComponent* component : components) {
		component->Disable();
	}
}

void GameObject::OnDestroy() {
	for (IComponent* component : components) {
		component->Destroy();
	}

	for (IComponent* component : components) {
		delete component;
	}

	components.clear();
}

void GameObject::SetActive(bool beActive) {
	/* Mike version */
	if (beActive == true) {
		gameObjectsToEnableMap[id] = this;
		return;
	}
	gameObjectsToDisableMap[id] = this;
	return;
}

bool GameObject::IsActive() {
	return {gameObjects.count(id) == 1 || gameObjectsToEnable.count(id) == 1};
}

//Static stuff
void GameObject::Init() {
	std::map<int, GameObject*>::iterator it;
	for (it = gameObjectsToInit.begin(); it != gameObjectsToInit.end(); it++) {
		it->second->OnInit();
	}
	gameObjectsToInit.clear();
}

void GameObject::Enable() {
	/* Mike Version */

	std::map<int, GameObject*>::iterator iterator;
	for (iterator = gameObjectsToEnableMap.begin(); iterator != gameObjectsToEnableMap.end(); iterator++) {
		iterator->second->OnEnable();
		activeGameObjects[iterator->first] = iterator->second;
	}
	gameObjectsToEnableMap.clear();
}


void GameObject::Disable() {
	/* Mike Version */

	std::map<int, GameObject*>::iterator iterator;
	for (iterator = gameObjectsToDisableMap.begin(); iterator != gameObjectsToDisableMap.end(); iterator++) {
		iterator->second->OnDisable();
		if (activeGameObjects.count(iterator->first) == 1) {
			activeGameObjects.erase(iterator->first);
		}
	}
	gameObjectsToDisableMap.clear();
}

void GameObject::Update(float deltaTime) {
	std::map<int, GameObject*>::iterator it;
	for (it = activeGameObjects.begin(); it != activeGameObjects.end(); it++) {
		it->second->OnUpdate(deltaTime);
	}
}

void GameObject::Draw(SDL_Renderer* renderer) {
	std::map<int, GameObject*>::iterator it;
	for (it = activeGameObjects.begin(); it != activeGameObjects.end(); it++) {
		it->second->OnDraw(renderer);
	}
}


void GameObject::Destroy(GameObject* gameObject, Predef predef) {
	if (Factory::Destroy(gameObject, predef) == false) {
		gameObjectsToDestroy[gameObject->id] = gameObject;
	}
}


void GameObject::CleanUp() {
	std::map<int, GameObject*>::iterator it;
	for (it = gameObjectsToDestroy.begin(); it != gameObjectsToDestroy.end(); it++) {
		if (activeGameObjects.count(it->first) == 1) {

			activeGameObjects.erase(it->first);
		}
		//it->second->OnDisable();
		it->second->OnDestroy();
		ClearOut(it);
		//std::cout << std::endl;
		//std::cout << "Just destroyed: " << it->second->ToString();
		//std::cout << std::endl;
		delete it->second;
	}
	gameObjectsToDestroy.clear();
}

void GameObject::ClearOut(std::map<int, GameObject*>::iterator it) {
	gameObjects.erase(it->second->id);
	gameObjectsDisabled.erase(it->second->id);
	gameObjectsToEnable.erase(it->second->id);
	gameObjectsToDisable.erase(it->second->id);
}