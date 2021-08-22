#pragma once

#include <Component/Core/IComponent.h>

class Transform: public IComponent {
public:
	Transform(GameObject* gameObject) : IComponent(gameObject) { }

	Vector2& Position() { return position; }
	double& Rotation() { return rotation; }
	Vector2& Scale() { return scale; }

	float& X() { return position.X; }
	float& Y() { return position.Y; }

	void Translate(Vector2 move);
	Vector2 forward;

protected:
	void OnInit() override;
	void OnEnable() override { }
	void OnUpdate(float deltaTime) override;
	void OnDisable() override { }
	void OnDestroy() override { }

private:
	Vector2 position;
	double rotation;
	Vector2 scale;
};