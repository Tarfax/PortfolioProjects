using UnityEngine;

namespace _Game.Scripts.Pathfinding
{
    [RequireComponent(typeof(BoxCollider))]
    public class GraphUpdater : MonoBehaviour
    {
        private Bounds bounds;

        private void Awake()
        {
            bounds = GetComponent<Collider>().bounds;
        }

        public void UpdateGraph()
        {
            AstarPath.active.UpdateGraphs(bounds);
        }
    }
}
