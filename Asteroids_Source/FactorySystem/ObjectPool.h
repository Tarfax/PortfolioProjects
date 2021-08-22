#pragma once


#include "FactorySystem/Predef.h"

#include <set>
#include <map>

enum PoolType {
	AsteroidLvl1Pool,
	AsteroidLvl2Pool,
	AsteroidLvl3Pool,
	ProjectilePool,
	End_DoNotUse
};

class FactoryObject;
class GameObject;
class ObjectPool {
public:
	ObjectPool() { }

	static void Init();

	static GameObject* FetchObject(PoolType pType);

	

	static void SetPoolSize(PoolType pType, const int poolSize);

	static ObjectPool* Create();

private:

	static ObjectPool* instance;
	static std::map<PoolType, std::set<GameObject*>> pools;
	//bool poolCanExtend{ false };

	static GameObject* CreateObject(PoolType pType);


public:
	//Mike
	static GameObject* FetchObject(Predef predef);
	static bool PoolObject(GameObject* gameObject, Predef definition);

	static void Destroy();

	static std::map<Predef, std::set<int>> predefIdPool;
	static std::map<int, GameObject*> idGameObjectPool;
};