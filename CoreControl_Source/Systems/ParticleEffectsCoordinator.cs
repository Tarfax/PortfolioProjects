using MC_Utility;
using System.Collections.Generic;
using UnityEngine;

public class ParticleEffectsCoordinator : MonoBehaviour {
    private Dictionary<GameObject, float> particlesToKill;
    private int activeParticlesCount = 0;
    private Dictionary<ParticleEffectType, Queue<GameObject>> objectPool;
    private Dictionary<ParticleEffectType, ParticleData> particleEffects;
    [SerializeField] private List<ParticleData> particles = default;

    private void Start() {
        objectPool = new Dictionary<ParticleEffectType, Queue<GameObject>>();
        particlesToKill = new Dictionary<GameObject, float>();
        particleEffects = new Dictionary<ParticleEffectType, ParticleData>();

        particleEffects.Add(ParticleEffectType.None, null);

        foreach (ParticleData item in particles) {
            particleEffects.Add(item.ParticleEffectType, item);
            objectPool.Add(item.ParticleEffectType, new Queue<GameObject>());
        }

        EventSystem<EnemyDeathEvent>.RegisterListener(OnAIDeathEvent);
        EventSystem<PlatformDeathEvent>.RegisterListener(OnPlatformDeathEvent);
        EventSystem<HitEvent>.RegisterListener(OnHitEvent);
    }

    private void Update() {
        if (activeParticlesCount > 0) {
            float deltaTime = Time.deltaTime;
            bool disableParticles = false;

            Dictionary<GameObject, float> temp = new Dictionary<GameObject, float>(particlesToKill);
            foreach (var key in temp.Keys) {
                particlesToKill[key] -= deltaTime;
                if (particlesToKill[key] <= 0f) {
                    disableParticles = true;
                }
            }

            if (disableParticles == true) {
                Dictionary<GameObject, float> temp2 = new Dictionary<GameObject, float>(particlesToKill);
                foreach (GameObject key in temp2.Keys) {
                    if (temp2[key] <= 0f) {
                        particlesToKill.Remove(key);
                        ParticleEffectType type = key.GetComponent<ParticleEffectsPlayer>().ParticleEffectType;
                        objectPool[type].Enqueue(key);
                        key.SetActive(false);
                    }
                }
                activeParticlesCount = particlesToKill.Count;
            }
        }
    }

    private void OnDisable() {
        EventSystem<EnemyDeathEvent>.UnregisterListener(OnAIDeathEvent);
        EventSystem<PlatformDeathEvent>.UnregisterListener(OnPlatformDeathEvent);
        EventSystem<HitEvent>.UnregisterListener(OnHitEvent);
    }

    private void OnAIDeathEvent(EnemyDeathEvent data) {
        if (data.ParticleEffectType == ParticleEffectType.None) {
            return;
        }
        GameObject gameObject = GetParticleOfType(data.ParticleEffectType, particleEffects[data.ParticleEffectType].ParticleEffectVisual);
        gameObject.SetActive(true);
        gameObject.transform.position = data.Position;
        gameObject.transform.rotation = Random.rotation;
        PlayParticle(gameObject, particleEffects[data.ParticleEffectType].ParticleDuration);

        
    }

    private void OnPlatformDeathEvent(PlatformDeathEvent data) {
        if (data.ParticleEffectType == ParticleEffectType.None) {
            return;
        }
        GameObject gameObject = GetParticleOfType(data.ParticleEffectType, particleEffects[data.ParticleEffectType].ParticleEffectVisual);
        gameObject.SetActive(true);
        gameObject.transform.position = data.Position;
        gameObject.transform.rotation = Random.rotation;
        PlayParticle(gameObject, particleEffects[data.ParticleEffectType].ParticleDuration);
    }

    private void OnHitEvent(HitEvent data) {

        if (data.ParticleEffectType == ParticleEffectType.None) {
            return;
        }
        GameObject gameObject = GetParticleOfType(data.ParticleEffectType, particleEffects[data.ParticleEffectType].ParticleEffectVisual);
        gameObject.SetActive(true);
        gameObject.transform.position = data.HitData.RaycastHit.point;
        Vector3 direction = data.HitData.RaycastHit.point - data.HitData.SourcePosition;
        if (direction != Vector3.zero) {
            gameObject.transform.rotation = Quaternion.LookRotation(direction.normalized);
        }
        PlayParticle(gameObject, particleEffects[data.ParticleEffectType].ParticleDuration);
    }

    private GameObject GetParticleOfType(ParticleEffectType type, GameObject objectToInstatiate) {
        if (objectPool[type].Count > 0) {
            return objectPool[type].Dequeue();
        }
        else {
            GameObject gameObject = Instantiate(objectToInstatiate);
            gameObject.GetComponent<ParticleEffectsPlayer>().ParticleEffectType = type;
            return gameObject;
        }
    }

    private void PlayParticle(GameObject gameObject, float duration) {
        ParticleEffectsPlayer pep = gameObject.GetComponent<ParticleEffectsPlayer>();
        pep.PlayParticles();
        particlesToKill.Add(gameObject, duration);
        activeParticlesCount = particlesToKill.Count;
    }

    public static ParticleEffectType GetParticleEffectType(ProjectileType projectileType) {
        switch (projectileType) {
            case ProjectileType.NormalProjectile:
                return ParticleEffectType.Impact_NormalProjectile;
            case ProjectileType.SniperProjectile:
                return ParticleEffectType.Impact_SniperProjectile;
            case ProjectileType.BossProjectile:
            default:
                return ParticleEffectType.Impact_BossProjectile;
        }
    }

    public static ParticleEffectType GetParticleEffectType(DamageType damageType) {
        switch (damageType) {
            case DamageType.SimpleProjectile:
                return ParticleEffectType.Impact_NormalProjectile;
            case DamageType.SniperProjectile:
                return ParticleEffectType.Impact_SniperProjectile;
            case DamageType.BossProjectile:
            default:
                return ParticleEffectType.Impact_BossProjectile;
        }
    }


}
