using MC_Utility;
using System;
using UnityEngine;
using UnityEngine.EventSystems;

[Serializable]
public class AI_ShootingSimpleProjectile : AI_Shooting {
    public override AI_ShootingType ShootingType { get => AI_ShootingType.SimpleProjectile; }
    private float fireRateTimer;

    private float shootAnimationTimer;

    private bool isAnimationTriggered = false;

    protected override void OnInitialize() {
        fireRateTimer = fireRate;
        isAnimationTriggered = false;
        shootAnimationTimer = shootingAnimationTime;
    }

    protected override void OnUpdate() {
        fireRateTimer -= deltaTime;
        if (fireRateTimer <= 0f) {
            CanShoot = true;
            if (IsAllowedToShoot == true) {
                shootAnimationTimer -= Time.deltaTime;

                if (shootAnimationTimer <= 0f) {
                    Fire();
                    fireRateTimer = fireRate;
                    IsAllowedToShoot = false;
                    CanShoot = false;
                    shootAnimationTimer = shootingAnimationTime;
                    isAnimationTriggered = false;
                }
                else if (isAnimationTriggered == false) {
                    animator.SetTrigger("Shoot");
                    isAnimationTriggered = true;
                }
            }
        }
    }

    private void Fire() {
        Projectile_SimpleProjectile projectile = UnityEngine.Object.Instantiate(projectileData.AIProjectileSettings.ProjectileVisual, transform.position, Quaternion.Euler(new Vector3(0f, 0f, 180f))).AddComponent<Projectile_SimpleProjectile>();
        projectile.SetProjectileData(GetProjectileBehaviour(), projectileData);
        EventSystem<FireEvent>.FireEvent(GetFireEvent());
    }

    private FireEvent GetFireEvent() {
        return new FireEvent() {
            fireSounds = projectileData.EnemyShootSounds,
        };
    }

    private ProjectileData.Behaviour GetProjectileBehaviour() {
        return projectileData.AIProjectileSettings;
    }

}