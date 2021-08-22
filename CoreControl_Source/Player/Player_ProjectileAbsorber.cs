using MC_Utility;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.InputSystem;

public class Player_ProjectileAbsorber : MonoBehaviour {

    private Queue<ProjectileData> projectilesCollected;

    [SerializeField] private ProjectileData projectileData;
    [SerializeField] private int projectileCount = 50;
    public int ProjectileCount { get => projectileCount; }

    [SerializeField] private int amountToAbsorb = 50;
    public int AmountToAbsorb { get => amountToAbsorb; }

    private Animator anim;

    [SerializeField] private GameObject gunParticleChild;
    private ParticleSystem particles;

    [SerializeField] private AudioSource[] shootAudio;
    [SerializeField] private AudioSource pickupAudio;

    private bool hasUnlimitedAmmo = false;
    public bool HasUnlimitedAmmo { get => hasUnlimitedAmmo; }

    private void Awake() {
        projectilesCollected = new Queue<ProjectileData>();
        anim = GetComponent<Animator>();
        particles = gunParticleChild.GetComponent<ParticleSystem>();
        gunParticleChild.SetActive(true);
        projectileCount = 0;
    }

    private void Update() {
        if (Keyboard.current.f1Key.wasPressedThisFrame == true) {
            hasUnlimitedAmmo = !hasUnlimitedAmmo;
        }
        gunParticleChild.transform.position = anim.GetBoneTransform(HumanBodyBones.LeftHand).position;
    }

    private void OnTriggerEnter(Collider other) {
        if (projectileCount < amountToAbsorb) {
            if (other.gameObject.layer == Layers.ProjectileLayerNumber) {
                Projectile projectile = other.gameObject.GetComponent<Projectile>();
                projectileData = projectile.ProjectileData;
                projectilesCollected.Enqueue(projectile.ProjectileData);
                projectile.Pickup();
                projectileCount++;

                if (particles.isPlaying) {
                    particles.Stop();
                }
                particles.Play();

                SoundPlayer.Instance.PlaySound(pickupAudio);

                EventSystem<PlayerAbsorbedProjectileEvent>.FireEvent(null);
            }
        }
    }

    public void Fire(Vector2 fireDirection, Vector2 spawnOffset) {
        if (projectileCount >= 1 || hasUnlimitedAmmo == true) {
            if (hasUnlimitedAmmo == false)
                projectileCount--;
            Quaternion direction = Quaternion.LookRotation(Vector3.forward, fireDirection);

            ProjectileData projectileData = GetNextProjectileData();

            Projectile_SimpleProjectile projectile = Instantiate(projectileData.PlayerProjectileSettings.ProjectileVisual, transform.position + (Vector3)spawnOffset, direction).AddComponent<Projectile_SimpleProjectile>();
            projectile.SetProjectileData(GetProjectileBehaviour(), projectileData);
            projectile.gameObject.layer = 14;

            foreach (var item in projectile.GetComponentsInChildren<Transform>()) {
                item.gameObject.layer = 14;
            }

            if (projectileCount <= 0) {
                particles.Stop();
            }

            SoundPlayer.Instance.PlayRandomSound(shootAudio);
        }
    }

    private ProjectileData GetNextProjectileData() {
        if (projectilesCollected.Count > 0) {
            return projectilesCollected.Dequeue();
        }
        else {
            return projectileData;
        }
    }

    private ProjectileData.Behaviour GetProjectileBehaviour() {
        return projectileData.PlayerProjectileSettings;
    }

}
