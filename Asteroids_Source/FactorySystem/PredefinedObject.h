#pragma once
#include <FactorySystem/ObjectDefinitions.h>
#include <FactorySystem/Predef.h>
#include <map>
#include <iostream> 
class PredefinedObject {
public:

	static void Init() {
		pdAsteroid1();
		pdAsteroid2();
		pdAsteroid3();
		pdPlayer();
		pdProjectile();
		pdAsteroidExplosion();
		pdUFO();
		pUFOExplosion();
		pdUFOProjectile();
	}

	static void AddPredefData(Predef def, ObjectData* data) {
		predefData[def] = data;
	}

	static ObjectData* GetPredef(Predef def) {
		return predefData[def];
	}

	static void Destroy() {
		std::map<Predef, ObjectData*>::iterator it;
		for (it = predefData.begin(); it != predefData.end(); ++it)	{
			delete it->second;
		}
		predefData.clear();
	}

private:
	PredefinedObject();

	static void pdAsteroid1() {
		Predef pre = Predef::Asteroid_Lvl1;
		AsteroidData* data = new AsteroidData();
		data->TextureIds.push_back("res/Sprites/asteroid_lvl01_1.png");
		data->RotationSpeed = 50.0f;
		data->MinSpeed = 50.0f;
		data->MaxSpeed = 70.0f;
		data->def = pre;
		data->Scale = 1.0f;
		data->Level = 1;

		AddPredefData(pre, data);
	}

	static void pdAsteroid2() {
		Predef pre = Predef::Asteroid_Lvl2;
		AsteroidData* data = new AsteroidData();
		data->TextureIds.push_back("res/Sprites/asteroid_lvl02_1.png");
		data->RotationSpeed = 33.0f;
		data->MinSpeed = 70.0f;
		data->MaxSpeed = 110.0f;
		data->def = pre;
		data->Scale = 1.0f;
		data->Level = 2;

		AddPredefData(pre, data);
	}

	static void pdAsteroid3() {
		Predef pre = Predef::Asteroid_Lvl3;
		AsteroidData* data = new AsteroidData();
		data->TextureIds.push_back("res/Sprites/asteroid_lvl03_1.png");
		data->RotationSpeed = 25.0f;
		data->MinSpeed = 70.0f;
		data->MaxSpeed = 140.0f;
		data->def = pre;
		data->Scale = 1.0f;
		data->Level = 3;

		AddPredefData(pre, data);
	}

	static void pdPlayer() {
		Predef pre = Predef::Player;
		PlayerData* data = new PlayerData();
		data->TextureIds.push_back("res/Sprites/ship.png");
		data->MaxSpeed = 700.0f;
		data->Acceleration = 0.15f;
		data->AccelFalloffStart = 70.0f;
		data->MomentumFalloff = 0.9999f;
		data->RotationSpeed = 270.0f;
		data->FireRate = 0.17f;
		data->def = pre;

		AddPredefData(pre, data);
	}


	static void pdProjectile() {
		Predef pre = Predef::Projectile;
		ProjectileData* data = new ProjectileData();
		data->TextureIds.push_back("res/Sprites/projectile.png");
		data->Speed = 450.0f;
		data->LifeTime = 1.25f;
		data->def = pre;
		data->Scale = 0.2f;

		AddPredefData(pre, data);
	}

	static void pdUFOProjectile() {
		Predef pre = Predef::UFOProjectile;
		ProjectileData* data = new ProjectileData();
		data->TextureIds.push_back("res/Sprites/UFOProjectile.png");
		data->Speed = 500.0f;
		data->LifeTime = 1.5f;
		data->def = pre;
		data->Scale = 0.8f;
		data->RotationSpeed = 100;

		AddPredefData(pre, data);
	}

	static void pdAsteroidExplosion() {
		Predef pre = Predef::AsteroidExplosion;
		ParticleEffectData* data = new ParticleEffectData();
		data->def = pre;
		data->TextureIds.push_back("res/Sprites/asteroidExplosion.png");

		data->MinLifeTime = 1;
		data->MaxLifeTime = 3;

		data->EmissionIntervall = 0;
		data->Repeat = false;
		data->StartOnActivation = true;

		data->Amount = 50;

		data->MinVelocityX = -200;
		data->MinVelocityY = -200;
		data->MaxVelocityX = 200;
		data->MaxVelocityY = 200;

		data->MinPositionOffsetX = -2;
		data->MinPositionOffsetY = -2;
		data->MaxPositionOffsetX = 2;
		data->MaxPositionOffsetY = 2;

		AddPredefData(pre, data);
	}
	static void pdUFO() {
		Predef pre = Predef::UFO;
		UFOData* data = new UFOData();
		data->TextureIds.push_back("res/Sprites/UFO.png");
		data->Speed = 100.0f;
		data->PositionRangeMin = 60.0f;
		data->PositionRangeMax = 400.0f;
		data->MagnitudeRangeMin = 40.0f;
		data->MagnitudeRangeMax = 60.0f;
		data->FrequencyRangeMin = 0.02f;
		data->FrequencyRangeMax = 0.04f;
		data->ProjectileSpeed = 1.0f;
		data->def = pre;

		AddPredefData(pre, data);
	}

	static void pUFOExplosion() {
		Predef pre = Predef::UFOExplosion;
		ParticleEffectData* data = new ParticleEffectData();
		data->def = pre;
		data->TextureIds.push_back("res/Sprites/asteroidExplosion.png");
		data->TextureIds.push_back("res/Sprites/UFOExplosion01.png");
		data->TextureIds.push_back("res/Sprites/UFOExplosion02.png");
		data->TextureIds.push_back("res/Sprites/UFOExplosion03.png");

		data->MinLifeTime = 1;
		data->MaxLifeTime = 3;

		data->EmissionIntervall = 0;
		data->Repeat = false;
		data->StartOnActivation = true;

		data->Amount = 15;

		data->MinVelocityX = -45;
		data->MinVelocityY = -45;
		data->MaxVelocityX =  45;
		data->MaxVelocityY =  45;

		data->MinPositionOffsetX = -10;
		data->MinPositionOffsetY = -10;
		data->MaxPositionOffsetX = 10;
		data->MaxPositionOffsetY = 10;

		AddPredefData(pre, data);
	}

	static std::map<Predef, ObjectData*> predefData;
};


