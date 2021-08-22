
using UnityEngine;

[SerializeField]
public interface IMovement {
    AI_MovementType MovementType { get; }
    void Update(float deltaTime);
    void LateUpdate();
    void Initialize(Transform _, Animator __, float positionOffset);
    void Destroy();
    void SetMoveSpeed(float newSpeed);

}
