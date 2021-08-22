using MC_Utility;
using System;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;

[Serializable]
public class AI_SideToSideMovement : AI_Movement {

    public override AI_MovementType MovementType { get => AI_MovementType.SideToSide; }

    [SerializeField] private Vector3 direction;
    private AI_Path currentPath;

    private PlatformHealthPoint targetPlatform;

    private float speed;

    protected override void OnInitialize() {
        currentPath = AI_Grid.GetSniperPath();
        direction = (currentPath.EndPosition - transform.position).normalized;
    }

    protected override void OnUpdate() {
        GetTarget();
        Move();
    }

    private void Move() {
        float xValue = transform.position.x;

        if (Mathf.Approximately(xValue, targetPlatform.transform.position.x) == false) {
            Vector3 targetPosition = targetPlatform.transform.position;
            targetPosition.y = currentPath.EndPosition.y;
            direction = (targetPosition - transform.position).normalized;

            Vector3 velocity = direction * deltaTime * moveSpeed;
            transform.position += velocity;
        }
    }

    private void GetTarget() {
        if (targetPlatform == null) {
            List<PlatformHealthPoint> hps = PlatformHealthPoint.GetLowestHitPlatformPoints(AI_Behaviour.GetEnemyOfType(Self.EnemyType).Count);

            float xValue = transform.position.x;

            float lowestValue = float.MaxValue;

            PlatformHealthPoint target = null;

            foreach (PlatformHealthPoint platform in hps) {
                if (platform.IsTargeted == false) {
                    float distance = Mathf.Abs(xValue - platform.transform.position.x);
                    if (distance < lowestValue) {
                        lowestValue = distance;
                        target = platform;
                    }
                }
            }

            if (target != null) {
                EventSystem<PlatformDeathEvent>.RegisterListener(OnPlatformDeath);

                targetPlatform = target;
                targetPlatform.IsTargeted = true;
            }
        }
    }

    private void OnPlatformDeath(PlatformDeathEvent deathEvent) {
        if (deathEvent.HealthPoint == targetPlatform) {
            targetPlatform = null;
        }
    }

    protected override void OnDestroy() {
        EventSystem<PlatformDeathEvent>.UnregisterListener(OnPlatformDeath);
        if (targetPlatform != null) {
            targetPlatform.IsTargeted = false;
        }
    }

}
