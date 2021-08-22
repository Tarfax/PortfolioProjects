using UnityEngine;

namespace _Game.Scripts.Systems.Spline
{
    public class AskForClosest : MonoBehaviour
    {
        [SerializeField] private float tResult;
        [SerializeField] private int curveNumber;
        [SerializeField] private Vector3 position;
        [SerializeField] private BezierSpline spline;

        private void OnValidate()
        {
            //position = spline.GetPointClosestTo(transform.position, out tResult, out curveNumber);
        }

        private void OnDrawGizmos()
        {
            Gizmos.color = Color.green;
            Gizmos.DrawWireSphere(position, 0.3f);
            Gizmos.color = Color.cyan;
            Gizmos.DrawWireSphere(spline.GetPoint(tResult), 0.2f);
        }
    }
}