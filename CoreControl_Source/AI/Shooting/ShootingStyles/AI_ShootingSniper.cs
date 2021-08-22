//using System;
//using UnityEngine;

//[Serializable]
//public class AI_ShootingSniper : AI_Shooting {
//    public override AI_ShootingType ShootingType { get => AI_ShootingType.Sniper; }
//    private float fireRateTimer;

//    private float startShootAnimationTime = 0.3f;
//    private float startShootAnimationTimer;

//    private bool isAnimationTriggered = false;

//    protected override void OnInitialize() {
//        fireRateTimer = fireRate;
//        isAnimationTriggered = false;
//        startShootAnimationTimer = startShootAnimationTime;
//    }

//    protected override void OnUpdate() {
//        fireRateTimer -= deltaTime;
//        if (fireRateTimer <= 0f) {
//            CanShoot = true;
//            if (IsAllowedToShoot == true) {
//                startShootAnimationTimer -= Time.deltaTime;

//                if (startShootAnimationTimer <= 0f) {
//                    Fire();
//                    fireRateTimer = fireRate;
//                    IsAllowedToShoot = false;
//                    CanShoot = false;
//                    startShootAnimationTimer = startShootAnimationTime;
//                    isAnimationTriggered = false;
//                }
//                else if (isAnimationTriggered == false) {
//                    animator.SetTrigger("Shoot");
//                    isAnimationTriggered = true;
//                }
//            }
//        }
//    }

//    private void Fire() {
//        Projectile_SimpleProjectile projectile = UnityEngine.Object.Instantiate(projectileData.AIProjectileSettings.ProjectileVisual, transform.position, Quaternion.Euler(new Vector3(0f, 0f, 180f))).AddComponent<Projectile_SimpleProjectile>();
//        projectile.SetProjectileData(GetProjectileBehaviour(), projectileData);
//    }

//    private ProjectileData.Behaviour GetProjectileBehaviour() {
//        return projectileData.AIProjectileSettings;
//    }

//}
