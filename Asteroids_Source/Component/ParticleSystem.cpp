#include "ParticleSystem.h"
#include "Objects/GameObject.h"
#include <EventSystem/ObjectEvent.h>
#include <Component/Core/BoxCollider2D.h>

void ParticleSystem::OnInit() {
	activeParticles.reserve(maxParticles);
	inactiveParticles.reserve(maxParticles);
}

void ParticleSystem::OnDestroy() {
	for (int i = 0; i < activeParticles.size(); i++) {
		delete activeParticles[i];
	}

	for (int i = 0; i < inactiveParticles.size(); i++) {
		delete inactiveParticles[i];
	}

	FireParticleDestroyedEvent();
}

void ParticleSystem::OnSetData(ObjectData* data) {
	ParticleEffectData* explosionData = dynamic_cast<ParticleEffectData*>(data);
	for (int i = 0; i < data->TextureIds.size(); i++)  	{
		Sprite* sprite = Sprite::CreateSprite(data->TextureIds[i]);
		sprites.push_back(sprite);
	}

	ParticleData particleData;
	particleData.EmissionIntervall = explosionData->EmissionIntervall;
	particleData.Repeat = explosionData->Repeat;
	particleData.StartOnActivation = explosionData->StartOnActivation;

	EmitData emitData;
	emitData.Amount = explosionData->Amount;

	emitData.MinLifeTime = explosionData->MinLifeTime;
	emitData.MaxLifeTime = explosionData->MaxLifeTime;

	emitData.MinVelocity.X = explosionData->MinVelocityX;
	emitData.MinVelocity.Y = explosionData->MinVelocityY;
	emitData.MaxVelocity.X = explosionData->MaxVelocityX;
	emitData.MaxVelocity.Y = explosionData->MaxVelocityX;

	emitData.MinPositionOffset.X = explosionData->MinPositionOffsetX;
	emitData.MinPositionOffset.Y = explosionData->MinPositionOffsetY;
	emitData.MaxPositionOffset.X = explosionData->MaxPositionOffsetX;
	emitData.MaxPositionOffset.Y = explosionData->MaxPositionOffsetY;

	particleData.Data = emitData;

	SetParticleData(particleData);
}


void ParticleSystem::OnUpdate(float deltaTime) {

	if (startOnActivation == true) {
		Emit();
		startOnActivation = false;
	}

	if (isEmitting == true) {
		std::vector<int> deadParticles;

		std::vector<Particle*>::iterator it;
		int counter = 0;
		for (it = activeParticles.begin(); it != activeParticles.end(); ++it) {

			Particle* p = *it;
			p->Update(deltaTime);
			if (p->IsDead() == true) {
				inactiveParticles.push_back(p);
				it = activeParticles.erase(activeParticles.begin() + counter);
				if (it == activeParticles.end()) {
					break;
				}
			}
			counter++;
		}

		if (activeParticles.empty() == true) {
			isEmitting = false;
		}

	}
	else if (isEmitting == false && repeat == false) {
		GameObject::Destroy(gameObject, Predef::AsteroidExplosion);
	}

	if (repeat == true) {
		nextLevelTimer -= deltaTime;
		if (nextLevelTimer < 0.0f) {
			Emit();
			SetEmissionTime();
		}
	}
}

void ParticleSystem::Emit() {

	Vector2 thisPosition = transform->Position();

	Vector2 position;
	Vector2 velocity;
	float lifeTime;

	for (int i = 0; i < data.Amount; i++) {
		if (activeParticles.size() >= maxParticles) {
			break;
		}

		position.X = thisPosition.X + (Mathf::RandomFloat(data.MinPositionOffset.X, data.MaxPositionOffset.X));
		position.Y = thisPosition.Y + (Mathf::RandomFloat(data.MinPositionOffset.Y, data.MaxPositionOffset.Y));

		velocity.X = Mathf::RandomFloat(data.MinVelocity.X, data.MaxVelocity.X);
		velocity.Y = Mathf::RandomFloat(data.MinVelocity.Y, data.MaxVelocity.Y);

		float lifeTime = Mathf::RandomFloat(data.MinLifeTime, data.MaxLifeTime);

		Particle* particle;
		if (inactiveParticles.size() > 0) {
			particle = inactiveParticles[0];
			inactiveParticles.erase(inactiveParticles.begin());
			particle->OnSetData(position, velocity, lifeTime);
		}
		else {
			particle = new Particle(position, velocity, lifeTime);
		}

		activeParticles.push_back(particle);
	}
	isEmitting = true;
}

void ParticleSystem::SetParticleData(ParticleData data) {
	this->data = data.Data;
	emissionIntervall = data.EmissionIntervall;
	repeat = data.Repeat;
	startOnActivation = data.StartOnActivation;
	SetEmissionTime();

}

void ParticleSystem::SetEmissionTime() {
	time = Mathf::RandomFloat(0, emissionIntervall);
	nextLevelTimer = time;
}

void ParticleSystem::OnDraw(SDL_Renderer* renderer) {
	if (isEmitting == true) {
		Sprite* sprite = sprites[(int)Mathf::RandomFloat(0, sprites.size() - 1)];
		SDL_Rect& source = sprite->Rect;
		SDL_Rect destination;
		SDL_Texture* texture = sprite->Texture;
		for (int i = 0; i < activeParticles.size(); i++) {
			Particle& p = *activeParticles[i];
			destination = {(int)p.position.X, (int)p.position.Y, source.w, source.h};
			SDL_RenderCopyEx(renderer, texture, &source, &destination, 0, nullptr, SDL_FLIP_NONE);
		}
	}
}

void ParticleSystem::OnRendererDisable() {
	for (int i = 0; i < activeParticles.size(); i++) {
		delete activeParticles[i];
	}
	activeParticles.clear();
	sprites.clear();
	FireParticleDestroyedEvent();
}

void ParticleSystem::FireParticleDestroyedEvent() {
	ParticleDestroyedEvent event(gameObject, gameObject->GetComponent<BoxCollider2D>(), predefData);
	FireDestroyedEvent(event);
}