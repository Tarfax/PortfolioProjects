#pragma once

#include <FactorySystem/FactoryObject.h>
#include <FactorySystem/ObjectPool.h>
#include "FactorySystem/PredefinedObject.h"
#include <Objects/GameObject.h>
class GameObject;

class Factory {

public:

	template <typename T, typename = std::enable_if_t<std::is_base_of<FactoryObject, T>::value>>
	static GameObject* Create(Predef predef) {

		GameObject* gameObject = ObjectPool::FetchObject(predef);
		T* component = nullptr;

		//Mike version
		if (gameObject == nullptr) {
			gameObject = new GameObject();
			component = gameObject->AddComponent<T>();
		}
		else {
			component = gameObject->GetComponent<T>();
			gameObject->SetActive(true);
		}

		component->SetData(PredefinedObject::GetPredef(predef));
		return gameObject;
	}

	//Mike version
	static bool Destroy(GameObject* gameObject, Predef predef) {
		if (ObjectPool::PoolObject(gameObject, predef) == true) {
			gameObject->SetActive(false);
			return true;
		}
		return false;
	}

	static void Destroy() {
		ObjectPool::Destroy();
	}

private:

	//Object pool here?

};