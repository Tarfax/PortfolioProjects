using MC_Utility;
using System;
using System.Collections.Generic;
using UnityEngine;

public class AIHealthPoint : HealthPoint {

    [SerializeField] private float destructionAnimationTime = 0.2f;
    [SerializeField] private float damageCooldown = 0.2f;
    private float damageCooldownTimer;

    private bool isHit;
    private float onHitAnimationTime = 0.4f;

    [SerializeField] private AudioSource[] hitSounds;

    private void Start() {
        damageCooldownTimer = damageCooldown;
    }

    private void Update() {
        if (isHit == true) {
            damageCooldownTimer -= Time.deltaTime;
            if (damageCooldownTimer <= 0f) {
                isHit = false;
                damageCooldownTimer = damageCooldown;
                GetComponent<Animator>().SetBool("IsHit", false);
            }
        }
    }

    public override void DoDamage(HitData hitData) {
        if (IsAlive == true) {
            isHit = true;
            
            EventSystem<HitEvent>.FireEvent(HitEventData(hitData));

            GetComponent<Animator>().SetBool("IsHit", true);
            GetComponent<Animator>().SetFloat("HitAnimation", UnityEngine.Random.Range(0, 2));

            healthPoints -= hitData.Damage;
            SoundPlayer.Instance.PlayRandomSound(hitSounds);

            if (healthPoints <= 0) {
                GetComponent<AI_Behaviour>().Kill(0f);
                IsAlive = false;
            }
        }
    }


    public EnemyDeathEvent DeathEventData() {
        EnemyDeathEvent deathEvent = new EnemyDeathEvent() {
            GameObject = gameObject,
            Position = transform.position,
            ParticleEffectType = ParticleEffectType.EnemyDeath,
            EnemyType = GetComponent<AI_Behaviour>().EnemyType,
            HealthPoint = this,
            AI_Behaviour = GetComponent<AI_Behaviour>(),


        };
        return deathEvent;
    }

    public HitEvent HitEventData(HitData bulletHit) {
        HitEvent deathEvent = new HitEvent() {
            HitData = bulletHit,
            ParticleEffectType = GetParticleEffectType(bulletHit.DamageType),
            Damage = bulletHit.Damage
        };
        return deathEvent;
    }

    private ParticleEffectType GetParticleEffectType(DamageType damageType) {
        switch (damageType) {
            case DamageType.SimpleProjectile:
                return ParticleEffectType.Impact_NormalProjectile;
            case DamageType.SniperProjectile:
            default:
                return ParticleEffectType.Impact_SniperProjectile;
        }
    } 

    [Serializable]
    private struct PlatformColor {
        public Color Color;
        public int Health;
    }

}
