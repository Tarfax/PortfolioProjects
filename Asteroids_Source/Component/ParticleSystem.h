#pragma once
#include <SDL.h>
#include <Structs/Particle.h>
#include <Component/Core/Renderer.h>
#include <FactorySystem/FactoryObject.h>

class ParticleSystem : public Renderer , public FactoryObject {

public:
	ParticleSystem(GameObject* gameObject): Renderer(gameObject) { gameObject->name = "ParticleSystem"; }

protected:
	void OnInit() override;
	void OnRendererEnable() override {}
	void OnUpdate(float deltaTime) override;
	void OnRendererDisable() override;
	void OnDestroy() override;
	void OnDraw(SDL_Renderer* renderer) override;

	void OnSetData(ObjectData* data) override;

private:
	void SetParticleData(ParticleData data);
	void SetEmissionTime();
	void Emit();
	void FireParticleDestroyedEvent();

	float time;
	float nextLevelTimer;

	float emissionIntervall;
	bool startOnActivation;

	bool repeat;

	int maxParticles = 100;

	bool isEmitting;

	std::vector <Sprite*> sprites;

	EmitData data;
	std::vector<Particle*> activeParticles;
	std::vector<Particle*> inactiveParticles;

	
};