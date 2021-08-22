#pragma once
#include <Structs/Vector2.h>
#include <Structs/Sprite.h>
#include <iostream>

struct EmitData {
	int Amount;

	float MinLifeTime;
	float MaxLifeTime;

	Vector2 MinVelocity;
	Vector2 MaxVelocity;

	Vector2 MinPositionOffset;
	Vector2 MaxPositionOffset;
};

struct ParticleData {
	bool	 Repeat;
	bool     StartOnActivation;
	float	 EmissionIntervall;
	EmitData Data;
};

struct Particle {

	Particle(Vector2 position, Vector2 velocity, float lifeTime)
	: position(position), velocity(velocity), lifeTime(lifeTime)
	{ }

	Particle(const Particle& other) {
		this->position = other.position;
		this->velocity = other.velocity;
		this->lifeTime = other.lifeTime;
	}

	//~Particle() { std::cout << "Particle died " << lifeTime << std::endl; }

	void Draw();
	void Update(float deltaTime);
	bool IsDead();

	void OnSetData(Vector2 position, Vector2 velocity, float lifeTime);

	Vector2 position;
	Vector2 velocity;
	float lifeTime;

};