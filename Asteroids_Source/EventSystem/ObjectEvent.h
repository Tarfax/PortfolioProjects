#pragma once
#include <EventSystem/Event.h>
#include <sstream>
#include <FactorySystem/Predef.h>
#include <Objects/GameObject.h>

class BoxCollider2D;

class ObjectEvent: public Event {
public:
	GameObject* gameObject;
	BoxCollider2D* collider;

	EventClassCategory(EventCategoryGameObject)
protected:
	ObjectEvent(GameObject* gameObject, BoxCollider2D* collider)
		: gameObject(gameObject), collider(collider) { }

};

class ObjectSpawnedEvent: public ObjectEvent {
public:
	ObjectSpawnedEvent(GameObject* gameObject, BoxCollider2D* collider)
		: ObjectEvent(gameObject, collider)
	{
	}

	std::string ToString() const override {
		std::stringstream ss;
		ss << "ObjectSpawnedEvent: " << gameObject->id << " (spawned)";
		return ss.str();
	}

	EventClassType(ObjectSpawned)

private:

};

class ObjectDestroyedEvent: public ObjectEvent {
public:
	ObjectDestroyedEvent(GameObject* gameObject, BoxCollider2D* collider)
		: ObjectEvent(gameObject, collider)
	{
	}

	std::string ToString() const override {
		std::stringstream ss;
		ss << "ObjectDestroyedEvent: " << gameObject->id << " (was destroyed)";
		return ss.str();
	}

	EventClassType(ObjectDestroyed)

private:

};

class AsteroidDestroyedEvent: public ObjectDestroyedEvent {
public:
	int Level;
	Predef predef;

	AsteroidDestroyedEvent(GameObject* gameObject, BoxCollider2D* collider, int level, Predef predef)
		: ObjectDestroyedEvent(gameObject, collider), Level(level), predef(predef)
	{
	}

	std::string ToString() const override {
		std::stringstream ss;
		ss << "AsteroidDestroyedEvent: " << gameObject->id << " (was destroyed)";
		return ss.str();
	}


	EventClassType(AsteroidDestroyed)

private:

	//Other stuff
	int sound;

};


class PlayerDestroyedEvent: public ObjectDestroyedEvent {
public:
	Predef predef;

	PlayerDestroyedEvent(GameObject* gameObject, BoxCollider2D* collider, Predef predef)
		: ObjectDestroyedEvent(gameObject, collider), predef(predef)
	{
	}

	std::string ToString() const override {
		std::stringstream ss;
		ss << "PlayerDestroyedEvent: " << gameObject->id << " (was destroyed)";
		return ss.str();
	}

	EventClassType(PlayerDestroyed)

private:

	//Other stuff
	int sound;

};

class ParticleDestroyedEvent: public ObjectDestroyedEvent {
public:
	Predef predef;

	ParticleDestroyedEvent(GameObject* gameObject, BoxCollider2D* collider, Predef predef)
		: ObjectDestroyedEvent(gameObject, collider), predef(predef)
	{
	}

	std::string ToString() const override {
		std::stringstream ss;
		ss << "ParticleDestroyedEvent: " << gameObject->id << " (was destroyed)";
		return ss.str();
	}

	EventClassType(ParticleDestroyed)

private:

	//Other stuff
	int sound;

};


class UFODestroyedEvent: public ObjectDestroyedEvent {
public:
	Predef predef;
	bool isDeathByPlayer;

	UFODestroyedEvent(GameObject* gameObject, BoxCollider2D* collider, Predef predef, bool deadByPlayer)
		: ObjectDestroyedEvent(gameObject, collider), predef(predef), isDeathByPlayer(deadByPlayer)
	{
	}

	std::string ToString() const override {
		std::stringstream ss;
		ss << "UFODestroyedEvent: " << gameObject->id << " (was destroyed)";
		return ss.str();
	}

	EventClassType(UFODestroyed)

private:

	//Other stuff
	int sound;

};