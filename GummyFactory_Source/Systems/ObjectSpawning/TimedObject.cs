using System.Collections;
using UnityEngine;

namespace _Game.Scripts.Systems.ObjectSpawning
{
    public class TimedObject : MonoBehaviour
    {
        [SerializeField] private float timeTillDespawn = 1f;
        [SerializeField] private bool startTimerOnAwake;

        public float TimeTillDespawn
        {
            get => timeTillDespawn;
            set => timeTillDespawn = value;
        }

        private void Awake()
        {
            if(startTimerOnAwake)
                StartTimer();
        }

        public void StartTimer()
        {
            StartCoroutine(TimedDespawn());
        }

        private IEnumerator TimedDespawn()
        {
            yield return new WaitForSeconds(timeTillDespawn);
            Despawn();
        }

        private void Despawn()
        {
            Destroy(gameObject);
        }
    }
}