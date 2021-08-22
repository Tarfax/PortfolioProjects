using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public abstract class HealthPoint : MonoBehaviour
{
    public bool IsAlive { get; protected set; } = true;

    [SerializeField] protected int healthPoints;

    public abstract void DoDamage(HitData hitData);

}
