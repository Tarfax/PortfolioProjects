
using UnityEngine;

public class EnemySpawnerDummy : MonoBehaviour
{
    public GameObject enemy;
    public float spawnRate;
    private float spawnRateTimer;

    private void Start()
    {
        // Set timer
        spawnRateTimer = spawnRate;
    }

    private void Update()
    {
        spawnRateTimer -= Time.deltaTime;
        if (spawnRateTimer <= 0f)
        {
            // Instantiate
            Instantiate(enemy, transform.position, Quaternion.identity);
            // Reset timer
            spawnRateTimer = spawnRate;
        }
    }
}
