using MC_Utility;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.InputSystem;

public class AI_Controller : MonoBehaviour {

    private Dictionary<EnemyType, List<AI_Behaviour>> enemyTypeBehaviour = default;
    private List<AI_Behaviour> enemies = default;

    private float normalFireRateTimer = default;
    private float normalFireRate = 2.5f;
    [Header("Normal Enemies")]
    [SerializeField] private float minNormalFireRate = 1.0f;
    [SerializeField] private float maxNormalFireRate = 2.5f;
    private float sniperFireRateTimer = default;
    private float sniperFireRate = 2.5f;
    [Header("Sniper Enemies")]
    [SerializeField] private float minSniperFireRate = 1.0f;
    [SerializeField] private float maxSniperFireRate = 2.5f;
    private float bossFireRateTimer = default;
    private float bossFireRate = 0.5f;
    [Header("Boss Enemies")]
    [SerializeField] private float minBossFireRate = 0.5f;
    [SerializeField] private float maxBossFireRate = 1.0f;

    private void OnEnable() {
        enemyTypeBehaviour = new Dictionary<EnemyType, List<AI_Behaviour>>();
        enemies = new List<AI_Behaviour>();
        EventSystem<EnemySpawnedEvent>.RegisterListener(OnEnemySpawned);
        EventSystem<EnemyDeathEvent>.RegisterListener(OnEnemyDeath);
    }

    private void Start() {
        normalFireRateTimer = normalFireRate;
    }

    private void Update() {
        normalFireRateTimer -= Time.deltaTime;
        sniperFireRateTimer -= Time.deltaTime;
        bossFireRateTimer -= Time.deltaTime;

        LetAIFire(ref normalFireRateTimer, ref normalFireRate, minNormalFireRate, maxNormalFireRate, EnemyType.BasicAlien);
        LetAIFire(ref sniperFireRateTimer, ref sniperFireRate, minSniperFireRate, maxSniperFireRate, EnemyType.SniperAlien);
        LetAIFire(ref bossFireRateTimer, ref bossFireRate, minBossFireRate, maxBossFireRate, EnemyType.BossAlien);

        Cheat_KillEnemy();
        Cheat_KillAllEnemy();
    }

    private void OnDisable() {
        EventSystem<EnemySpawnedEvent>.UnregisterListener(OnEnemySpawned);
        EventSystem<EnemyDeathEvent>.UnregisterListener(OnEnemyDeath);
    }

    private void LetAIFire(ref float timer, ref float fireRate, float min, float max, EnemyType type) {
        if (timer <= 0f) {
            if (enemyTypeBehaviour.ContainsKey(type) == true) {
                if (enemyTypeBehaviour[type].Count > 0) {
                    int randomIndex = Random.Range(0, enemyTypeBehaviour[type].Count);
                    AI_Behaviour behaviour = enemyTypeBehaviour[type][randomIndex];
                    if (behaviour.CanShoot == true) {
                        behaviour.IsAllowedToShoot = true;
                        fireRate = Random.Range(min, max);
                        timer = fireRate;
                    }
                }
            }
        }
    }

    private void Cheat_KillEnemy() {
        if (Keyboard.current.kKey.wasPressedThisFrame == true) {
            if (enemyTypeBehaviour.Count > 0) {
                for (int i = 0; i < enemyTypeBehaviour.Count;) {
                    enemies[i].Kill();
                    return;
                }
            }
        }
    }

    private void Cheat_KillAllEnemy() {
        if (Keyboard.current.lKey.wasPressedThisFrame == true) {
            if (enemyTypeBehaviour.Count > 0) {
                for (int i = 0; i < enemyTypeBehaviour.Count; i++) {
                    enemies[i].Kill();
                }
            }
        }
    }

    private void OnEnemySpawned(EnemySpawnedEvent spawnedEvent) {
        if (enemyTypeBehaviour.ContainsKey(spawnedEvent.EnemyType) == false) {
            enemyTypeBehaviour.Add(spawnedEvent.EnemyType, new List<AI_Behaviour>());
        }
        enemyTypeBehaviour[spawnedEvent.EnemyType].Add(spawnedEvent.AI_Behaviour);
        enemies.Add(spawnedEvent.AI_Behaviour);
    }

    private void OnEnemyDeath(EnemyDeathEvent deathEvent) {
        enemyTypeBehaviour[deathEvent.EnemyType].Remove(deathEvent.AI_Behaviour);
        enemies.Remove(deathEvent.AI_Behaviour);
    }

}
