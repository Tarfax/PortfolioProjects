using System;
using UnityEngine;

[Serializable]
public class AI_MovementBehaviour {

    private Transform transform = default;
    private Animator animator = default;

    [Header("Movement Settings")]
    [SerializeField] private float positionOffset = 0f;
    [SerializeField] private float moveSpeed = 3.0f;
    private float startMoveSpeed;

    [Header("Movement Style")]
    [SerializeField] private AI_MovementType movementType = default;
    [Space]

    private IMovement movement = default;

    public void Start(Transform transform, Animator anim) {
        this.transform = transform;
        animator = anim;
        SelectMovementStyle();
        startMoveSpeed = moveSpeed;
    }

    public void SelectMovementStyle() {
        switch (movementType) {

            case AI_MovementType.SineWave:
                movement =  new AI_SineWaveMovement();
                break;
            case AI_MovementType.SideToSide:
                movement = new AI_SideToSideMovement();
                break;

        }

        movement.Initialize(transform, animator, positionOffset);
        movement.SetMoveSpeed(moveSpeed);
    }

    public void Update(float deltaTime) {
        movement.Update(deltaTime);
    }

    public void LateUpdate() {
        movement.LateUpdate();
    }

    public void Destroy() {
        movement.Destroy();
    }

#if UNITY_EDITOR
    public void OnValidate() {
        if (movement != null && movementType != movement.MovementType) {
            SelectMovementStyle();
        }

        if (startMoveSpeed != moveSpeed && Application.isPlaying == true && movement != null) {
            movement.SetMoveSpeed(moveSpeed);
        }
    }

    public void OnDrawGizmo(Transform transform) {
        
    }
#endif

}
