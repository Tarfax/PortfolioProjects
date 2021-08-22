#include "CollisionGrid.h"
#include <Component/Core/BoxCollider2D.h>
#include <Objects/GameObject.h>
#include <iostream>

void CollisionGrid::AddCollider(BoxCollider2D* collider) {
	Cell* cell = GetCell(collider->GetOrigin());
	AddCollider(collider, cell);
}

void CollisionGrid::AddCollider(BoxCollider2D* collider, Cell* cell) {
	cell->colliders.push_back(collider);
	collider->gridCell = cell;
	collider->cellVectorIndex = cell->colliders.size() - 1;
}

void CollisionGrid::RemoveCollider(BoxCollider2D* collider) {
	std::vector<BoxCollider2D*>& colliders = collider->gridCell->colliders;

	colliders[collider->cellVectorIndex] = colliders.back();
	colliders.pop_back();

	if (collider->cellVectorIndex < colliders.size()) {
		colliders[collider->cellVectorIndex]->cellVectorIndex = collider->cellVectorIndex;
	}
	collider->cellVectorIndex = -1;
	collider->gridCell = nullptr;
}

CollisionGrid::CollisionGrid(int width, int height, int cellSize)
	: width(width), height(height), cellSize(cellSize)
{
	xCells = ceil((float)width / cellSize);
	yCells = ceil((float)height / cellSize);


	cells.resize(yCells * xCells);

	for (size_t i = 0; i < cells.size(); i++) { 
		cells[i].cell = i;
	}
}

Cell* CollisionGrid::GetCell(int x, int y) {

	if (x < 0) {
		x = 0;
	}

	if (x >= xCells) {
		x = xCells - 1;
	}

	if (y < 0) {
		y = 0;
	}

	if (y >= yCells) {
		y = yCells - 1;
	}

	return &cells[y * xCells + x];
}

Cell* CollisionGrid::GetCell(const Vector2& position) {
	int cellX = (int)(position.X / cellSize);
	int cellY = (int)(position.Y / cellSize);

	return GetCell(cellX, cellY);
}