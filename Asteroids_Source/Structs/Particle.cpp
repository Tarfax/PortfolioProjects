#include "Particle.h"

void Particle::Draw() {
	
}

void Particle::Update(float deltaTime) {
	position += velocity * deltaTime;
	lifeTime -= deltaTime;
}

bool Particle::IsDead() {
	return lifeTime < 0.0f;
}

void Particle::OnSetData(Vector2 position, Vector2 velocity, float lifeTime) {
	this->position = position;
	this->velocity = velocity;
	this->lifeTime = lifeTime;
}
