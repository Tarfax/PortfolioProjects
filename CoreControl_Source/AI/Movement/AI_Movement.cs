using System;
using UnityEngine;

[Serializable]
public abstract class AI_Movement : IMovement {

    public abstract AI_MovementType MovementType { get; }

    protected Transform transform;
    protected Animator animator;

    private AI_Behaviour self;
    protected AI_Behaviour Self {
        get {
            if (self == null) {
                self = transform.GetComponent<AI_Behaviour>();
            }
            return self;
        }
    }

    protected float deltaTime;
    protected float time;

    protected float moveSpeed = 4.0f;
    protected float positionOffset;

    public void Initialize(Transform transform, Animator anim, float positionOffset) {
        animator = anim;
        this.transform = transform;
        this.positionOffset = positionOffset;
        OnInitialize();
    }

    protected virtual void OnInitialize() { }

    public void Update(float deltaTime) {
        this.deltaTime = deltaTime;
        time += deltaTime;

        OnUpdate();
    }

    protected virtual void OnUpdate() { }

    public void LateUpdate() {
        OnLateUpdate();
    }

    protected virtual void OnLateUpdate() { }

    public void SetMoveSpeed(float newSpeed) {
        moveSpeed = newSpeed;
    }

    public virtual void Destroy() {
        OnDestroy();
    }
    protected virtual void OnDestroy() { }

}
