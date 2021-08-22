//using UnityEngine;

//public class Projectile_SniperProjectile : Projectile {
//    protected override void Move(float deltaTime) {
//        transform.Translate(transform.up * projectileBehaviour.ProjectileSpeed * deltaTime, Space.World);
//    }

//    protected override void OnCollision(RaycastHit hit) {
//        HealthPoint hp = hit.collider.GetComponent<HealthPoint>();
//        if (hp != null && hp.IsAlive == true) {
//            hp.DoDamage(GetProjectileHitData(hit, projectileBehaviour.ProjectileDamage));
//            Destroy(gameObject);
//        }
//    }

//    protected override HitData GetProjectileHitData(RaycastHit hit, int projectileDamage) {
//        return new HitData() {
//            SourcePosition = startingPosition,
//            RaycastHit = hit,
//            DamageType = GetDamageType(projectileData.ProjectileType),
//            Damage = projectileDamage
//        };
//    }

//}
