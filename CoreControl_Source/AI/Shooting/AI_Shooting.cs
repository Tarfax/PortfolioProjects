using UnityEngine;

public abstract class AI_Shooting : IShooting {


    private AI_Behaviour behaviour;
    public bool IsAllowedToShoot {
        get {
            if (behaviour == null) {
                behaviour = transform.GetComponent<AI_Behaviour>();
            }
            return behaviour.IsAllowedToShoot;
        }
        set {
            if (behaviour == null) {
                behaviour = transform.GetComponent<AI_Behaviour>();
            }
            behaviour.IsAllowedToShoot = value;
        }
    }

    public bool CanShoot { get; protected set; }

    public abstract AI_ShootingType ShootingType { get; }

    protected float shootingAnimationTime = 0.17f;
    protected float fireRate = 0.8f;
    protected ProjectileData projectileData;
    protected Transform transform;
    protected Animator animator;
    protected float deltaTime;

    public void Initialize(Transform transform, Animator anim, ProjectileData data, float fireRate, float shootingAnimationTime) {
        this.transform = transform;
        animator = anim;
        projectileData = data;
        this.fireRate = fireRate;
        this.shootingAnimationTime = shootingAnimationTime;
        OnInitialize();
    }

    protected virtual void OnInitialize() { }

    public void Update(float deltaTime) {
        this.deltaTime = deltaTime;
        OnUpdate();
    }

    protected virtual void OnUpdate() { }

    public void LateUpdate() {
        OnLateUpdate();
    }

    protected virtual void OnLateUpdate() { }

    public virtual void Destroy() {
        OnDestroy();
    }

    protected virtual void OnDestroy() { }
}
