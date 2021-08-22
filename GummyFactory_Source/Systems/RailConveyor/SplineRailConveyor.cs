using System.Collections;
using _Game.Scripts.Systems.Spline;
using UnityEngine;

namespace _Game.Scripts.Systems.RailConveyor
{
    [RequireComponent(typeof(BezierSpline))]
    public class SplineRailConveyor : MonoBehaviour
    {
        [SerializeField] private float spawnRate = 1f;
        [SerializeField] private SpawnMode spawnMode = SpawnMode.Continuous;
        [SerializeField] private int setAmount;
        [SerializeField] private bool checkIfBlocked = true;
        [SerializeField] private SplineWalker walkerToSpawn;
        [SerializeField, Tooltip("Values lower than 0 ar ignored in favour of the prefabs own.")]
        private float customTravelDuration = -1f;
        [SerializeField, Tooltip("Values lower than 0 ar ignored in favour of the prefabs own.")] 
        private float customGrabRange = -1f;
        [SerializeField] private SplineWalkerMode mode = SplineWalkerMode.Destroy;

        private BezierSpline spline;

        private void Awake()
        {
            spline = GetComponent<BezierSpline>();
            StartCoroutine(SpawnCycle());
        }

        private IEnumerator SpawnCycle()
        {
            int amountSpawned = 0;
            WaitForSeconds delay = new WaitForSeconds(1f / spawnRate);
            while (true)
            {
                yield return delay;
                if (checkIfBlocked && Physics.CheckSphere(spline.GetPoint(0), 0.5f, -1, 
                    QueryTriggerInteraction.Ignore))
                    continue;
                
                SplineWalker walker = Instantiate(walkerToSpawn);
                walker.Spline = spline;
                walker.Mode = mode;

                if (customTravelDuration >= 0)
                    walker.Duration = customTravelDuration;
                if (customGrabRange >= 0)
                {
                    GrabberLowerer lowerer = walker.GetComponentInChildren<GrabberLowerer>();
                    if (lowerer != null)
                        lowerer.GrabRange = customGrabRange;
                }

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