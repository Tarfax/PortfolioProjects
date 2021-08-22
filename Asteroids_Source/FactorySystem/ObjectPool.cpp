#include "ObjectPool.h"
#include "FactorySystem/Factory.h"
#include "Component/Behaviour/Projectile.h"
#include "Component/Behaviour/Asteroid.h"

ObjectPool* ObjectPool::instance = nullptr;
std::map<PoolType, std::set<GameObject*>> ObjectPool::pools;



GameObject* ObjectPool::FetchObject(PoolType pType) {
	std::cout << "ObjectPool::FetchObject" << std::endl;

	for (GameObject* obj : ObjectPool::pools[pType]) {
		if (!obj->IsActive()) {
			return obj;
		}
	}

	GameObject* obj = CreateObject(pType);
	ObjectPool::pools[pType].emplace(obj);
	return obj;
}



void ObjectPool::SetPoolSize(PoolType pType, const int poolSize) {
	int pSize = ObjectPool::pools[pType].size();

	if (pSize < poolSize) {
		int diff = poolSize - pSize;

		for (int i = 0; i < diff; i++) {
			GameObject* obj = CreateObject(pType);
			//obj->SetActive(false);
			ObjectPool::pools[pType].emplace(obj);
		}
	}
	else if (pSize > poolSize) {
		int diff = pSize - poolSize;

		for (int i = 0; i < diff; i++) {
			std::set<GameObject*>::iterator it = ObjectPool::pools[pType].begin();
			GameObject::Destroy(*it, Predef::Unknown);
			ObjectPool::pools[pType].erase(it); // check if this is Ok.
		}
	}
}

ObjectPool* ObjectPool::Create() {
	if (instance == nullptr) {
		instance = new ObjectPool();
	}
	return instance;
}

void ObjectPool::Init() {
	return;

	for (int i = AsteroidLvl1Pool; i != End_DoNotUse; i++) {
		std::set<GameObject*> pool;
		PoolType pType = static_cast<PoolType>(i);
		std::pair<PoolType, std::set<GameObject*>> element(pType, pool);
		ObjectPool::pools.emplace(element);
		SetPoolSize(pType, 10);
	}
}

GameObject* ObjectPool::CreateObject(PoolType pType) {
	switch (pType) {
		case AsteroidLvl1Pool:
			return Factory::Create<Asteroid>(Predef::Asteroid_Lvl1);
			break;
		case AsteroidLvl2Pool:
			return Factory::Create<Asteroid>(Predef::Asteroid_Lvl2);
			break;
		case AsteroidLvl3Pool:
			return Factory::Create<Asteroid>(Predef::Asteroid_Lvl3);
			break;
		case ProjectilePool:
			return Factory::Create<Projectile>(Predef::Projectile);
			break;
		case End_DoNotUse:
			return nullptr;
			break;
	}
}


std::map<Predef, std::set<int>> ObjectPool::predefIdPool;
std::map<int, GameObject*> ObjectPool::idGameObjectPool;

// -- Mike version -- 

/// <summary>
/// Gets first best available pooled object of a predef
/// </summary>
/// <param name="predef: "> Type to fetch</param>
/// <returns>Returns a nullptr if nothing was fetched</returns>
GameObject* ObjectPool::FetchObject(Predef predef) {
	if (predefIdPool[predef].size() > 0) {

		int id = *predefIdPool[predef].begin();
		GameObject* pooledGameObject = idGameObjectPool[id];

		predefIdPool[predef].erase(predefIdPool[predef].begin());
		idGameObjectPool.erase(id);

		return pooledGameObject;
	}
	return nullptr;
}

/// <summary>
/// Places an object in the pool if we really really want to.
/// Uknown types does not pool.
/// </summary>
/// <param name="gameObject: ">Object to pool</param>
/// <param name="preDefinition: ">Predef ?</param>
/// <returns>returns true if it was pooled</returns>
bool ObjectPool::PoolObject(GameObject* gameObject, Predef predef) {
	if (predef == Predef::Unknown) {
		return false;
	}

	int id = gameObject->id;
	predefIdPool[predef].insert(id);
	idGameObjectPool[id] = gameObject;

	return true;
}

void ObjectPool::Destroy() {
	std::map<int, GameObject*>::iterator it;
	for (it = idGameObjectPool.begin(); it != idGameObjectPool.end(); ++it) {
		GameObject::Destroy(it->second);
	}

	predefIdPool.clear();
	idGameObjectPool.clear();
}