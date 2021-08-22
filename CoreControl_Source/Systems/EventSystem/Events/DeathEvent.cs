using UnityEngine;

public class DeathEvent : IEvent {
    public GameObject GameObject { get; set; }
    public ParticleEffectType ParticleEffectType { get; set; }
    public Vector3 Position { get; set; }
    public string SoundToPlay { get; set; }
    public float SoundCooldown { get; set; }


}

public class EnemyDeathEvent : DeathEvent {
    public AI_Behaviour AI_Behaviour { get; set; }
    public AIHealthPoint HealthPoint { get; set; }
    public EnemyType EnemyType { get; set; }
}

public class PlatformDeathEvent : DeathEvent {
    public PlatformHealthPoint HealthPoint { get; set; }
}
