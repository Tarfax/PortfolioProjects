#pragma once
#include <vector>
#include <Structs/Vector2.h>
#include <string>

class BoxCollider2D;
struct Cell {
	std::vector<BoxCollider2D*> colliders;
	int cell;
	std::string ToString() { return std::to_string(cell); }
};

class CollisionGrid {
	friend class Physics;
public:
	CollisionGrid(int width, int height, int cellSize);
	~CollisionGrid() {

	}

	Cell* GetCell(int x, int y);
	Cell* GetCell(const Vector2& position);

	void AddCollider(BoxCollider2D* collider);
	void AddCollider(BoxCollider2D* collider, Cell* cell);

	void RemoveCollider(BoxCollider2D* collider);

private:
	std::vector<Cell> cells;
	int width;
	int height;
	int cellSize;

	int xCells;
	int yCells;
};