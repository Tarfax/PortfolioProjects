#pragma once

#include <Objects/GameObject.h>
#include <Component/Core/IComponent.h>
#include <Component/Core/Transform.h>
//#include <vector>

//class GameObject;

class Renderer: public IComponent {
public:
	static void Draw(SDL_Renderer* renderer);
protected:

	Renderer(GameObject* gameObject);

	~Renderer();

	virtual void OnDraw(SDL_Renderer* renderer) = 0;

	Transform* transform;

	virtual void OnRendererEnable() { };
	virtual void OnRendererDisable() { };
	virtual void OnInit() { };
	virtual void OnUpdate(float deltaTime) { };
	virtual void OnDestroy() { };

private:
	void OnEnable() override;
	void OnDisable() override;


	static std::vector<Renderer*> renderers;
};