#pragma once
#include <Component/Core/BoxCollider2D.h>

class CollisionGrid;
class ObjectSpawnedEvent;
class ObjectDestroyedEvent;
class Event;

#define CellSize 64

class Physics {

public:
	void Destroy();
	static Physics* Init(int screenWidth, int screenHeight) {
		return new Physics(screenWidth, screenHeight);
	}

	void AddCollider(BoxCollider2D* collider);
	void RemoveCollider(BoxCollider2D* collider);

	void UpdateColliders();
	void CheckCollisions();


private:

	Physics(int width, int height);

	void OnEvent(Event& e);
	bool OnColliderSpawned(ObjectSpawnedEvent& e);
	bool OnColliderDestroyed(ObjectDestroyedEvent& e);

	void CheckCollision(BoxCollider2D* collider, std::vector<BoxCollider2D*>& collidersToCheck, int startingIndex);

	std::vector<BoxCollider2D*> colliders;
	CollisionGrid* grid;
};