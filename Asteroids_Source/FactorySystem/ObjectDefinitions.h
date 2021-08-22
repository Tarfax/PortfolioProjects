#pragma once

#include <vector>
#include <string>
#include <FactorySystem/Predef.h>

struct ObjectData {
public:
	ObjectData() { }
	virtual ~ObjectData() {}
	std::vector<std::string> TextureIds;
	Predef def;
};

struct AsteroidData: public ObjectData {
public:
	AsteroidData() { }

	float Scale;
	float MinSpeed;
	float MaxSpeed;
	float RotationSpeed;
	int Level;
};

struct PlayerData: public ObjectData {
public:
	PlayerData(): ObjectData() { }

	float MaxSpeed;
	float Acceleration;
	float AccelFalloffStart;
	float MomentumFalloff;
	float RotationSpeed;
	float FireRate;
};

struct ProjectileData: public ObjectData {
public:
	ProjectileData(): ObjectData() { }

	float Speed;
	float LifeTime;
	float Scale;
	float RotationSpeed;
};

struct ParticleEffectData: public ObjectData {
public:
	ParticleEffectData(): ObjectData() { }

	//Particle System Data
	float Amount = 100;
	bool Repeat = true;
	bool StartOnActivation = true;
	float EmissionIntervall = 2;

	//Particle Data
	float MinLifeTime = 1;
	float MaxLifeTime = 3;

	float MinVelocityX = -100;
	float MinVelocityY = -100;
	float MaxVelocityX = 200;
	float MaxVelocityY = 200;

	float MinPositionOffsetX = -0.1f;
	float MinPositionOffsetY = -0.1f;
	float MaxPositionOffsetX = 2;
	float MaxPositionOffsetY = 2;

};

struct UFOData : public ObjectData {
public:
	UFOData() : ObjectData() { }

	float Speed;
	float PositionRangeMin;
	float PositionRangeMax;
	float MagnitudeRangeMin;
	float MagnitudeRangeMax;
	float FrequencyRangeMin;
	float FrequencyRangeMax;
	float ProjectileSpeed;
};

struct MainMenuData: public ObjectData {
public:
	MainMenuData(): ObjectData() { }
	
	
};