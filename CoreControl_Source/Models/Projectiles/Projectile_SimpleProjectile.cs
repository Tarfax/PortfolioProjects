using System.Collections.Generic;
using UnityEngine;

public class Projectile_SimpleProjectile : Projectile {
    protected override void Move(float deltaTime) {
        if (isActive == true) {
            transform.Translate(transform.up * projectileBehaviour.ProjectileSpeed * deltaTime, Space.World);
        }
    }

    protected override void OnCollision(RaycastHit hit) {
        HealthPoint hp = hit.collider.GetComponent<HealthPoint>();
        if (hp != null && hp.IsAlive == true) {
            hp.DoDamage(GetProjectileHitData(hit, projectileBehaviour.ProjectileDamage));
            GetComponentInChildren<MeshRenderer>().enabled = false;
            KillProjectile();
        }
    }

    protected override HitData GetProjectileHitData(RaycastHit hit, int projectileDamage) {
        return new HitData() {
            SourcePosition = startingPosition,
            RaycastHit = hit,
            DamageType = GetDamageType(projectileData.ProjectileType),
            Damage = projectileDamage
        };
    }

}
