using System;
using UnityEngine;

[Serializable]
public class AI_SineWaveMovement : AI_Movement {

    public override AI_MovementType MovementType { get => AI_MovementType.SineWave; }

    private Vector3 direction;
    private bool isTransitioningRow = false;

    private AI_Path currentPath;

    private float moveAgainTimer;
    private float moveAgainTime = 0.15f;

    protected override void OnInitialize() {
        currentPath = AI_Grid.GetNearestPath(transform.position.y);
        direction = (currentPath.EndPosition - transform.position).normalized;
    }

    protected override void OnUpdate() {
        moveAgainTimer -= deltaTime;
        if (Self.IsAllowedToShoot == true && Self.CanShoot == true && Self.EnemyType == EnemyType.BossAlien) {
            moveAgainTimer = moveAgainTime;
        }
        else if (moveAgainTimer <= 0f) {
            Vector3 velocity;
            if (isTransitioningRow == true) {
                if (transform.position.y <= currentPath.StartPosition.y) {
                    isTransitioningRow = false;
                }
                else {
                    velocity = Vector3.down * deltaTime * moveSpeed;
                    transform.Translate(velocity, Space.World);
                }
                return;
            }

            velocity = direction * deltaTime * moveSpeed;
            transform.Translate(velocity, Space.World);

            if (currentPath.IsAtEndPosition(transform.position.x, positionOffset) == true) {
                currentPath = AI_Grid.GetPathAtRow(currentPath.Row + 1);
                isTransitioningRow = true;
                direction = (currentPath.EndPosition - currentPath.StartPosition).normalized;
            }
        }
    }

    protected override void OnDestroy() {
        
    }

}
