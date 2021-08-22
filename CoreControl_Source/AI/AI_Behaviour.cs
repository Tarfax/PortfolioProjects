using MC_Utility;
using System.Collections.Generic;
using UnityEngine;

public class AI_Behaviour : MonoBehaviour {

    public EnemyType EnemyType { get => enemyType; }
    public bool IsAllowedToShoot { get; set; }
    public bool CanShoot { get => shootingSettings.CanShoot; }

    private new Transform transform;
    private Animator animator;

    private static List<AI_Behaviour> enemies;
    private static Dictionary<EnemyType, List<AI_Behaviour>> enemieTypes;

    private static int spawnedAIs;
    private int ID;

#pragma warning disable 0649
    [SerializeField] private EnemyType enemyType;
    [SerializeField] private AI_MovementBehaviour movementBehaviour;
    [SerializeField] private AI_ShootingBehaviour shootingSettings;
    [SerializeField] private AudioSource killSound;
#pragma warning restore 0649

    private void OnEnable() {
        if (enemies == null) {
            enemies = new List<AI_Behaviour>();
            enemieTypes = new Dictionary<EnemyType, List<AI_Behaviour>>();
        }               
    }

    void Start() {
        IsAllowedToShoot = false;

        AddEnemyToList();

        spawnedAIs++;
        ID = spawnedAIs;
        gameObject.name = "Enemy " + enemyType.ToString() + " #" + ID;

        transform = base.transform;
        animator = GetComponent<Animator>();
        movementBehaviour.Start(transform, animator);
        shootingSettings.Start(transform, animator);

        EventSystem<EnemySpawnedEvent>.FireEvent(GetSpawnedEventData());
    }


    void Update() {
        float deltaTime = Time.deltaTime;
        movementBehaviour.Update(deltaTime);
        shootingSettings.Update(deltaTime);

        //Handle stuff if AI is below a certain threshold

    }

    private void LateUpdate() {
        movementBehaviour.LateUpdate();
        shootingSettings.LateUpdate();

    }


#if UNITY_EDITOR
    private void OnValidate() {
        if (Application.isPlaying) {
            movementBehaviour.OnValidate();
            shootingSettings.OnValidate();
        }
    }

    private void OnDrawGizmos() {
        movementBehaviour.OnDrawGizmo(transform);
        shootingSettings.OnDrawGizmo();
    }
#endif

    private void OnDestroy() {
        movementBehaviour.Destroy();
        enemies.Remove(this);
        enemieTypes[enemyType].Remove(this);
    }

    public void Kill(float animationTime = 0f) {
        SoundPlayer.Instance.PlaySound(killSound);
        EventSystem<EnemyDeathEvent>.FireEvent(GetDeathEventData());
        Destroy(gameObject, animationTime);
    }

    public EnemySpawnedEvent GetSpawnedEventData() {
        EnemySpawnedEvent spawnedEvent = new EnemySpawnedEvent() {
            GameObject = gameObject,
            Position = transform.position,
            EnemyType = enemyType,
            HealthPoint = GetComponent<HealthPoint>(),
            AI_Behaviour = this,

        };
        return spawnedEvent;
    }

    public EnemyDeathEvent GetDeathEventData() {
        EnemyDeathEvent deathEvent = new EnemyDeathEvent() {
            GameObject = gameObject,
            Position = transform.position,
            EnemyType = enemyType,
            HealthPoint = GetComponent<AIHealthPoint>(),
            AI_Behaviour = this,
            ParticleEffectType = ParticleEffectType.EnemyDeath
        };
        return deathEvent;
    }

    private void AddEnemyToList() {
        enemies.Add(this);
        if (enemieTypes.ContainsKey(enemyType) == false) {
            enemieTypes.Add(enemyType, new List<AI_Behaviour>());
        }
        enemieTypes[enemyType].Add(this);
    }

    public static List<AI_Behaviour> GetEnemyOfType(EnemyType type) {
        if (enemieTypes != null && enemieTypes.ContainsKey(type) == true) {
            return enemieTypes[type];
        }
        return null;
    }

}
