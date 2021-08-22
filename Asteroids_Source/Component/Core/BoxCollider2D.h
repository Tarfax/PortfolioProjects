#pragma once

#include "Component/Core/IComponent.h"
#include "Component/Core/Transform.h"
#include <SDL.h>
#include <Enums/Layer.h>

//#include <map>
struct Cell;


class BoxCollider2D: public IComponent {
	friend class CollisionGrid;
	friend class Physics;
public:
	BoxCollider2D(GameObject* gameObject);

	void OnInit() override;
	void OnEnable() override;
	void OnUpdate(float deltaTime) override;
	void OnDisable() override;
	void OnDestroy() override;

	void Draw(SDL_Renderer* renderer, Transform* transform);

	void SetBounds(int x, int y, int h, int w);
	void SetBounds(SDL_Rect rect);

	void SetLayer(Layer layer);

	inline Vector2 GetOrigin() { return Vector2 {(float)bounds.x + (bounds.w / 2), (float)bounds.y + (bounds.h / 2)}; }

	inline SDL_Rect GetBounds() const { return bounds; }
	inline void ResetBounds() { bounds = {(int)transform->X(), (int)transform->Y(), (int)(originalBounds.w * transform->Scale().X), (int)(originalBounds.h * transform->Scale().Y)}; }

	bool IsColliding(BoxCollider2D* other);

	inline Layer GetLayer() { return layer; }


private:
	Transform* transform;

	SDL_Rect bounds;
	SDL_Rect originalBounds;

	int id;
	static int nextId;

	Layer layer;

	static bool renderCollider;

	Cell* gridCell = nullptr;
	int cellVectorIndex = -1;

};