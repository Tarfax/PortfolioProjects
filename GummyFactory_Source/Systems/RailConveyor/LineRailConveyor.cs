using System.Collections;
using _Game.Scripts.Systems.Spline;
using UnityEngine;

namespace _Game.Scripts.Systems.RailConveyor
{
    [RequireComponent(typeof(Line))]
    public class LineRailConveyor : MonoBehaviour
    {
        [SerializeField] private float spawnRate = 1f;
        [SerializeField] private SpawnMode spawnMode = SpawnMode.Continuous;
        [SerializeField] private int setAmount;
        [SerializeField] private bool checkIfBlocked = true;
        [SerializeField] private LineWalker walkerToSpawn;
        [SerializeField] private LineWalker.EndBehaviourType endBehaviour = LineWalker.EndBehaviourType.Destroy;

        private Line line;

        private void Awake()
        {
            line = GetComponent<Line>();
            StartCoroutine(SpawnCycle());
        }

        private IEnumerator SpawnCycle()
        {
            int amountSpawned = 0;
            WaitForSeconds delay = new WaitForSeconds(spawnRate);
            while (true)
            {
                yield return delay;
                if (checkIfBlocked && Physics.CheckSphere(line.WorldPoint0, 0.5f, -1, QueryTriggerInteraction.Ignore))
                    continue;
                
                LineWalker walker = Instantiate(walkerToSpawn);
                walker.Line = line;
                walker.EndBehaviour = endBehaviour;

                if (spawnMode == SpawnMode.SetAmount)
                {
                    amountSpawned++;
                    if(amountSpawned == setAmount)
                        break;
                }
            }
        }
    }
}