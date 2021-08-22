
using UnityEngine;


[System.Serializable]
public struct EnemyKillScore
{                                          [Tooltip("The enemy to link score to.")] 
    public EnemyType enemy;                [Tooltip("The score for enemy kill.")]
    [Min(0f)] public int killScore;
}
