using System.Collections;
using UnityEngine;

namespace _Game.Scripts.Systems.ObjectSpawning
{
    public class ObjectSpawner : MonoBehaviour
    {
        [SerializeField] private float ratePerSecond;
        [SerializeField] private bool startCycleOnAwake;
        [SerializeField] private bool randomRotation;
        [SerializeField] private GameObject[] objectsToSpawn;

        [SerializeField, HideInInspector] private float timeBetweenSpawns;

        private Transform spawnTransform;

        private bool isRunning;

        private void OnValidate()
        {
            timeBetweenSpawns = 1 / ratePerSecond;
        }

        private void Awake()
        {
            spawnTransform = transform;
        }

        private void OnEnable()
        {
            if (startCycleOnAwake && isRunning == false)
                StartSpawningCycle();
        }

        private void OnDisable()
        {
            if(isRunning)
                StopSpawningCycle();
        }

        public void SpawnTimedObject()
        {
            Quaternion rotation = randomRotation
                ? Quaternion.Euler(Random.Range(0f, 360f), Random.Range(0f, 360f), Random.Range(0f, 360f))
                : spawnTransform.rotation;
            
            GameObject obj = Instantiate(objectsToSpawn[Random.Range(0, objectsToSpawn.Length)], 
                spawnTransform.position, rotation);
        }

        public void StartSpawningCycle()
        {
            if(isRunning == true)
                return;
            StartCoroutine(SpawnCycle());
        }

        public void StopSpawningCycle()
        {
            if(isRunning == false)
                return;
            StopAllCoroutines();
        }

        private IEnumerator SpawnCycle()
        {
            while (true)
            {
                yield return new WaitForSeconds(timeBetweenSpawns);

                SpawnTimedObject();
            }
        }
    }
}