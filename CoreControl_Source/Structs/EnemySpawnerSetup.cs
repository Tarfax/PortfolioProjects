
using UnityEngine;


[System.Serializable]
public struct GlobalIntensitySetup
{                                                      [Tooltip("After how many sec to change intensity.")] 
    [Min(0f)] public float globalIntensityTimePoint;   [Tooltip("The intensity to change to. 1 = default, 0 = pause")]
    [Min(0f)] public float globalIntensity;
}

[System.Serializable]
public struct SpawnSetup
{                                                      [Tooltip("Enemy to spawn.")]                 
    public GameObject enemy;                           [Tooltip("Wheter to spawn at top path or not.")]
    public bool spawnTop;                              [Tooltip("Wheter to spawn a limited number.")]
    [Space(8)]
    public bool spawnLimited;                          [Tooltip("The limited spawn count to spawn.")]
    public int spawnLimit;                             [Tooltip("Maximum number of active enemies.")]
    [Space(8)]
    public int spawnActiveMax;                         [Tooltip("Minimum time in sec before spawn rate begins.")]
    [Min(0f)] public float startDelayMin;              [Tooltip("Maximum time in sec before spawn rate begins.")]
    [Min(0f)] public float startDelayMax;              [Tooltip("Minimum time in sec between enemy spawns.")]
    [Min(0f)] public float spawnRateMin;               [Tooltip("Maximum time in sec between enemy spawns.")]
    [Min(0f)] public float spawnRateMax;               [Tooltip("Offset for the global intensifier.")]
    [Range(0f, 10f)] public float spawnRateIntensity;  [Tooltip("The chance for an enemy to spawn.")]
    [Range(0f, 1f)] public float spawnProbability;     [Tooltip("Increases the distance to nearest spawn point " +
                                                                "from the greatest wall distance. DISABLED")]
    [HideInInspector][Range(0f, 1f)] public float minimumAvoidance;
}
