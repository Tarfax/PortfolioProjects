using UnityEngine;

public class SpawnedEvent : IEvent {
    public GameObject GameObject { get; set; }
    public ParticleEffectType ParticleEffectType { get; set; }
    public Vector3 Position { get; set; }
    public string SoundToPlay { get; set; }
    public float SoundCooldown { get; set; }

}

public class EnemySpawnedEvent : SpawnedEvent {
    public AI_Behaviour AI_Behaviour { get; set; }
    public HealthPoint HealthPoint { get; set; }
    public EnemyType EnemyType { get; set; }

}