using UnityEngine;

public interface IShooting {
    AI_ShootingType ShootingType { get; }
    bool IsAllowedToShoot { get; set; }
    bool CanShoot { get; }
    void Initialize(Transform _mainT, Animator __, ProjectileData data, float fireRate, float shootingAnimationTime);
    void Update(float deltaTime);
    void LateUpdate();
    void Destroy();
}
