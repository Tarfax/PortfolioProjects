using System;
using UnityEngine;

[Serializable]
public class AI_ShootingBehaviour {
    public bool CanShoot { get => shootingType.CanShoot; }

    private Transform transform = default;
    private Animator animator;
    

    private AI_ShootingType targetingType = default;
    [Space]

    [SerializeField] private float shootingAnimationTime = 0.17f;
    [Range(0.1f, 6f)] [SerializeField] protected float fireRate = 0.8f;
    [Space]

    [SerializeField] ProjectileData projectileData;

    private IShooting shootingType = default;

    public void Start(Transform transform, Animator anim) {
        this.transform = transform;
        animator = anim;
        SelectTargetingStyle();
    }

    public void SelectTargetingStyle() {
        switch (targetingType) {
            case AI_ShootingType.SimpleProjectile:
                shootingType =  new AI_ShootingSimpleProjectile();
                break;
        }
        shootingType.Initialize(transform, animator, projectileData, fireRate, shootingAnimationTime);
    }

    public void Update(float deltaTime) {
        shootingType.Update(deltaTime);
    }

    public void LateUpdate() {
        shootingType.LateUpdate();
    }

#if UNITY_EDITOR
    public void OnValidate() {
        if (shootingType != null && targetingType != shootingType.ShootingType) {
            SelectTargetingStyle();
        }
    }

    public void Destroy() {
        shootingType.Destroy();
    }

    public void OnDrawGizmo() {
        
    }
#endif
}
