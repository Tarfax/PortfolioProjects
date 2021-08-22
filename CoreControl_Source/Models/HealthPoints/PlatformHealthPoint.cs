using MC_Utility;
using System;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.InputSystem;

public class PlatformHealthPoint : HealthPoint {

    public static int Count { get => platforms.Count; }
    [SerializeField] private List<PlatformColor> platformColors;

    [SerializeField] private MeshRenderer colorMesh;
    [SerializeField] private new Animation animation;
    [SerializeField] private AudioSource[] hitSounds;
    [SerializeField] private AudioSource[] collapseSounds;
    public bool IsTargeted { get; set; }

    private Material material;

    private static List<PlatformHealthPoint> platforms;
    private bool isUndestructable = false;

    private void Start() {
        if (platforms == null) {
            platforms = new List<PlatformHealthPoint>();
        }
        platforms.Add(this);
        animation = GetComponent<Animation>();
        material = colorMesh.materials[0];
       
        SetPlatformColor();
    }

    private void Update() {
        Cheat_MakeUndestructable();
    }

    private void OnDestroy() {
        if (platforms.Contains(this) == true) {
            platforms.Remove(this);
        }
    }

    public override void DoDamage(HitData hitData) {
        animation.Stop();
        if (IsAlive == true) {
            EventSystem<HitEvent>.FireEvent(HitEventData(hitData));
            healthPoints -= hitData.Damage;
            SetPlatformColor();
            animation.Play();
            SoundPlayer.Instance.PlayRandomSound(hitSounds);

            if (healthPoints <= 0) {
                Kill();
            }
        }
    }

    public void Kill() {
        if (isUndestructable == false) {
            EventSystem<PlatformDeathEvent>.FireEvent(DeathEventData());
            IsAlive = false;
            platforms.Remove(this);
            SoundPlayer.Instance.PlayRandomSound(collapseSounds);
            Destroy(gameObject, 0.1f);
        }
    }

    public PlatformDeathEvent DeathEventData() {
        PlatformDeathEvent deathEvent = new PlatformDeathEvent() {
            GameObject = gameObject,
            Position = transform.position,
            ParticleEffectType = ParticleEffectType.PlatformDeath
        };
        return deathEvent;
    }

    public HitEvent HitEventData(HitData bulletHit) {
        HitEvent deathEvent = new HitEvent() {
            HitData = bulletHit,
            ParticleEffectType = ParticleEffectsCoordinator.GetParticleEffectType(bulletHit.DamageType),
            

        };
        return deathEvent;
    }

    public static List<PlatformHealthPoint> GetLowestHitPlatformPoints(int enemyCount) {
        int lowestHealthPoint = int.MaxValue;
        foreach (PlatformHealthPoint item in platforms) {
            if (item.healthPoints < lowestHealthPoint && item.IsTargeted == false) {
                lowestHealthPoint = item.healthPoints;
            }
        }

        List<PlatformHealthPoint> lowestHealths = new List<PlatformHealthPoint>();

        foreach (PlatformHealthPoint item in platforms) {
            if (item.healthPoints == lowestHealthPoint) {
                bool shouldAdd = true;
                foreach (PlatformHealthPoint platformHealthPoint in lowestHealths) {
                    if (platformHealthPoint.transform.position.x == item.transform.position.x) {
                        shouldAdd = false;
                    }
                }
                if (shouldAdd == true) {
                    lowestHealths.Add(item);
                }
            }
        }

        if (enemyCount > lowestHealths.Count) {
            foreach (PlatformHealthPoint item in platforms) {
                lowestHealths.Add(item);
            }
        }

        return lowestHealths;
    }


    private void SetPlatformColor() {
        foreach (PlatformColor platformColor in platformColors) {
            if (platformColor.Health == healthPoints) {
                material.color = platformColor.Material.color;
                material.SetTexture("_MainTex", platformColor.Material.GetTexture("_MainTex"));
                material.SetTexture("_MetallicGlossMap", platformColor.Material.GetTexture("_MetallicGlossMap"));
                material.SetTexture("_EmissionMap", platformColor.Material.GetTexture("_EmissionMap"));
                material.SetTexture("_BumpMap", platformColor.Material.GetTexture("_BumpMap"));
                return;
            }
        }
    }

    private void Cheat_MakeUndestructable() {
        if (Keyboard.current.uKey.wasPressedThisFrame == true) {
            isUndestructable = !isUndestructable;
        }
    }


    [Serializable]
    private struct PlatformColor {
        public int Health;
        public Material Material;
    }

}
