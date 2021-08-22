#include "Physics.h"
#include <SpatialPartitioning/CollisionGrid.h>
#include <EventSystem/Event.h>
#include <EventSystem/ObjectEvent.h>

Physics::Physics(int width, int height) {
	grid = new CollisionGrid(width, height, CellSize);
	BoxCollider2D::AddOnSpawnedCallback(BindFunction(Physics::OnEvent, this));
	BoxCollider2D::AddOnDestroyCallback(BindFunction(Physics::OnEvent, this));
}

void Physics::Destroy() {
	BoxCollider2D::RemoveOnSpawnedCallback(BindFunction(Physics::OnEvent, this));
	BoxCollider2D::RemoveOnDestroyCallback(BindFunction(Physics::OnEvent, this));
	delete grid;
	colliders.clear();
}

void Physics::UpdateColliders() {

	for (size_t i = 0; i < colliders.size(); i++) {
		BoxCollider2D* collider = colliders[i];
		Cell* newCell = grid->GetCell(collider->GetOrigin());

		if (newCell != collider->gridCell) {
			grid->RemoveCollider(collider);
			grid->AddCollider(collider, newCell);
		}
	}

}

void Physics::CheckCollisions() {
	for (int i = 0; i < grid->cells.size(); i++) {
		int x = i % grid->xCells;
		int y = i / grid->xCells;

		Cell* cell = &grid->cells[i];

		for (int j = 0; j < cell->colliders.size(); j++) {
			BoxCollider2D* collider = cell->colliders[j];

			CheckCollision(collider, cell->colliders, j + 1);

			if (x > 0) {
				//Left cell
				CheckCollision(collider, grid->GetCell(x - 1, y)->colliders, 0);

				if (y > 0) {
					//Top Left cell
					CheckCollision(collider, grid->GetCell(x - 1, y - 1)->colliders, 0);
				}

				if (y < grid->yCells - 1) {
					//Bottom left cell
					CheckCollision(collider, grid->GetCell(x - 1, y + 1)->colliders, 0);
				}
			}

			if (y > 0) {
				//Up cell
				CheckCollision(collider, grid->GetCell(x, y - 1)->colliders, 0);
			}
		}

	}
}

void Physics::CheckCollision(BoxCollider2D* collider, std::vector<BoxCollider2D*>& collidersToCheck, int startingIndex) {
	for (int i = startingIndex; i < collidersToCheck.size(); i++) {
		if (collider->IsColliding(collidersToCheck[i]) == true) {
			collider->gameObject->OnCollision(collidersToCheck[i]);
			collidersToCheck[i]->gameObject->OnCollision(collider);
		}
	}
}

void Physics::OnEvent(Event& e) {
	EventDispatcher dispatcher(e);
	if (dispatcher.Dispatch<ObjectSpawnedEvent>(BindFunction(Physics::OnColliderSpawned, this)) == true) {
		return;
	}

	if (dispatcher.Dispatch<ObjectDestroyedEvent>(BindFunction(Physics::OnColliderDestroyed, this)) == true) {
		return;
	}
}

bool Physics::OnColliderSpawned(ObjectSpawnedEvent& e) {
	AddCollider(e.collider);
	return true;
}

bool Physics::OnColliderDestroyed(ObjectDestroyedEvent& e) {
	RemoveCollider(e.collider);
	return true;
}

void Physics::AddCollider(BoxCollider2D* collider) {

	grid->AddCollider(collider);
	colliders.push_back(collider);
}

void Physics::RemoveCollider(BoxCollider2D* collider) {
	for (size_t i = 0; i < colliders.size(); i++) {
		if (colliders[i] == collider) {
			colliders.erase(colliders.begin() + i);
			grid->RemoveCollider(collider);
			return;
		}
	}
}


