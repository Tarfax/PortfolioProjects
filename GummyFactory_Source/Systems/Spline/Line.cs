using UnityEngine;

namespace _Game.Scripts.Systems.Spline
{
    public class Line : MonoBehaviour
    {
        public Vector3 point0;
        public Vector3 point1;
        
        public Vector3 WorldPoint0 => transform.TransformPoint(point0);
        public Vector3 WorldPoint1 => transform.TransformPoint(point1);
    }
}
