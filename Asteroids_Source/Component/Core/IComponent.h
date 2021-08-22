#pragma once
#include <functional>
#include <vector>
#include <map>
#include <iostream>
#include <string>

#include <Math/Mathf.h>
#include <Structs/Vector2.h>
#include <Structs/Sprite.h>
#include <Core/Input.h>
#include <SoundSystem/SoundCoordinator.h>

class Event;
class GameObject;
class BoxCollider2D;
class IComponent {
	using EventCallbackFunc = std::function<void(Event&)>;

public:
	IComponent(GameObject* gameObject): gameObject(gameObject) {}
	virtual ~IComponent() {}
	void Init();
	void Enable();
	void Update(float deltaTime);
	void Disable();
	void Destroy();

	void Collision(BoxCollider2D* other);

	GameObject* gameObject;

protected:
	virtual void OnInit() = 0;
	virtual void OnEnable() = 0;
	virtual void OnUpdate(float deltaTime) = 0;
	virtual void OnDisable() = 0;
	virtual void OnDestroy() = 0;
	virtual void OnCollision(BoxCollider2D* other) { }


public: //Event System
	static inline void AddOnSpawnedCallback(const EventCallbackFunc& callback) {
		for (int i = 0; i < onObjectSpawned.size(); i++) {
			if (onObjectSpawned[i].EventCallback.target_type().hash_code() == callback.target_type().hash_code()) {
				return;
			}
		}

		onObjectSpawned.emplace_back(callback);
	}
	static inline void RemoveOnSpawnedCallback(const EventCallbackFunc& callback) {
		for (int i = 0; i < onObjectSpawned.size(); i++) {
			if (onObjectSpawned[i].EventCallback.target_type().hash_code() == callback.target_type().hash_code()) {
				onObjectSpawned.erase(onObjectSpawned.begin() + i);
				return;
			}
		}
	}
	
	static inline void AddOnDestroyCallback(const EventCallbackFunc& callback) {
		for (int i = 0; i < onObjectDestroyed.size(); i++) {
			if (onObjectDestroyed[i].EventCallback.target_type().hash_code() == callback.target_type().hash_code()) {
				return;
			}
		}

		onObjectDestroyed.emplace_back(callback);
	}
	static inline void RemoveOnDestroyCallback(const EventCallbackFunc& callback) {
		for (int i = 0; i < onObjectDestroyed.size(); i++) {
			if (onObjectDestroyed[i].EventCallback.target_type().hash_code() == callback.target_type().hash_code()) {
				onObjectDestroyed.erase(onObjectDestroyed.begin() + i);
				return;
			}
		}
	}


protected:
	void FireSpawnedEvent(Event& event) {
		for (int i = 0; i < onObjectSpawned.size(); i++) {
			CallbackData data = onObjectSpawned[i];
			data.EventCallback(event);
		}
	}

	void FireDestroyedEvent(Event& event) {
		for (int i = 0; i < onObjectDestroyed.size(); i++) {
			CallbackData data = onObjectDestroyed[i];
			data.EventCallback(event);
		}
	}

private: //EventSystem
	struct CallbackData {
		CallbackData() { }
		CallbackData(EventCallbackFunc callback): EventCallback(callback) { }

		EventCallbackFunc EventCallback;
	};

	static std::vector<CallbackData> onObjectDestroyed;
	static std::vector<CallbackData> onObjectSpawned;
};