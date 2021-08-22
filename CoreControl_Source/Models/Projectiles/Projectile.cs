using MC_Utility;
using UnityEngine;

public abstract class Projectile : MonoBehaviour {

    protected new Transform transform;
    protected ProjectileData projectileData;
    public ProjectileData ProjectileData => projectileData;

    protected ProjectileData.Behaviour projectileBehaviour;
    private Vector3 currentPosition;
    private Vector3 previousPosition;
    protected Vector3 startingPosition;
    protected Vector3 velocity;
    protected float magnitude;
    [SerializeField] private LayerMask layerMask;
    protected bool isColliding;

    private float raycastDistance = 0.3f;

    [SerializeField] protected bool isActive;

    private float projectileDelayDeath = 1f;

    private float timeUntilRemoval = 0.2f;

    [Tooltip("Hit detection is done via spherecast, uses this radius for hit detection width.")]
    [SerializeField] private float hitDetectionRadius = 0.5f;

    private void Start() {
        transform = base.transform;
        startingPosition = transform.position;
        isActive = true;
    }

    private void Update() {
        float deltaTime = Time.deltaTime;
        previousPosition = transform.position;
        if (isActive == true) {
            Move(deltaTime);
            CheckForCollision();
            CheckProjectileDistance();
        }
        velocity = (transform.position - previousPosition) / deltaTime;
        magnitude = velocity.magnitude * (deltaTime * 4f);

        if (isActive == false && magnitude == 0f && timeUntilRemoval <= 0f) {
            KillProjectile();
        } else if (isActive == false && magnitude == 0f) {
            timeUntilRemoval -= deltaTime;
        }

    }

    private void OnDrawGizmosSelected() {
        Transform transform = base.transform;

        Debug.DrawRay(transform.position + -transform.up * raycastDistance, transform.up * magnitude);

        if (Physics.Raycast(transform.position + -transform.up * raycastDistance, transform.up, magnitude)) {
            Debug.DrawRay(transform.position + -transform.up * raycastDistance, transform.up * magnitude, Color.green);
        }
    }

    protected abstract void Move(float deltaTime);
    protected abstract void OnCollision(RaycastHit hit);
    protected abstract HitData GetProjectileHitData(RaycastHit hit, int projectileDamage);

    public void SetProjectileData(ProjectileData.Behaviour behaviour, ProjectileData data) {
        projectileData = data;
        projectileBehaviour = behaviour;
        layerMask = behaviour.Target;
    }

    public void Pickup() {
        if (isActive == true) {
            isActive = false;
            KillProjectile();
        }
    }

    private void CheckProjectileDistance() {
        float distanceFromStart = Vector3.Distance(startingPosition, transform.position);
        if (distanceFromStart > projectileBehaviour.DistanceUntilDestroyed) {
            isActive = false;
            KillProjectile();
        }
    }

    protected void KillProjectile() {
        MakeInvisible();
        Destroy(gameObject, projectileDelayDeath);
    }

    private void MakeInvisible() {
        MeshRenderer[] meshRenderers = GetComponents<MeshRenderer>();
        MeshRenderer[] childMeshRenderers = GetComponentsInChildren<MeshRenderer>();
        MeshRenderer[] allRenderers = new MeshRenderer[meshRenderers.Length + childMeshRenderers.Length];
        meshRenderers.CopyTo(allRenderers, 0);
        childMeshRenderers.CopyTo(allRenderers, 0);
        if (allRenderers != null) {
            foreach (var item in allRenderers) {
                item.enabled = false;
            }
        }

        Collider collider = GetComponent<Collider>();
        if (collider != null) {
            collider.enabled = false;
        }

        ParticleSystem[] particleSystems = GetComponentsInChildren<ParticleSystem>();
        if (particleSystems != null) {
            foreach (var item in particleSystems) {
                item.Stop();
            }
        }
    }

    private void CheckForCollision() {

        RaycastHit hit;

        if (Physics.SphereCast(transform.position - transform.up * raycastDistance, hitDetectionRadius, transform.up, out hit, magnitude, layerMask)) {
            OnCollision(hit);
            hit.collider.GetComponent<DeathCheck>()?.Hit();
            isActive = false;
        }
        else if (Physics.SphereCast(transform.position - transform.up * raycastDistance, hitDetectionRadius, transform.up, out hit, magnitude, Layers.Wall)) {
            Vector3 newdirection = Vector3.Reflect(transform.up, hit.normal);
            transform.rotation = Quaternion.LookRotation(Vector3.forward, newdirection);
            EventSystem<HitEvent>.FireEvent(HitEventData(GetHitData(hit)));
        }
    }

    private HitEvent HitEventData(HitData bulletHit) {
        HitEvent deathEvent = new HitEvent() {
            HitData = bulletHit,
            ParticleEffectType = ParticleEffectsCoordinator.GetParticleEffectType(projectileData.ProjectileType),
        };
        return deathEvent;
    }

    private HitData GetHitData(RaycastHit hit) {
        return new HitData() {
            SourcePosition = startingPosition,
            RaycastHit = hit,
            DamageType = GetDamageType(projectileData.ProjectileType),
        };
    }

    public static DamageType GetDamageType(ProjectileType projectileType) {
        switch (projectileType) {
            case ProjectileType.NormalProjectile:
                return DamageType.SimpleProjectile;
            case ProjectileType.SniperProjectile:
                return DamageType.SniperProjectile;
            case ProjectileType.BossProjectile:
            default:
                return DamageType.BossProjectile;
        }
    }

}
