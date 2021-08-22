using System;
using _Game.Scripts.Systems.Interactables;
using Pathfinding;
using Pathfinding.Util;
using UnityEngine;

namespace _Game.Scripts.Actors
{
    [RequireComponent(typeof(Seeker))]
    public class PathFollower : ActorComponentBase
    {
        [SerializeField] private float pickNextWaypointDistance = 1.5f;
        [SerializeField] private float endReachedDistance = 0.1f;
        [SerializeField] private float slowdownDistance = 0.5f;

        private event Action OnTargetChanged;
        private event Action OnTargetReached;

        private bool reachedEndOfPath;
        private Vector3 targetPosition;
    
        private Seeker seeker;
        private Path path;

        private IMovementPlane movementPlane = GraphTransform.identityTransform;
        private PathInterpolator interpolator = new PathInterpolator();

        public float RemainingDistance => interpolator.valid ? 
            interpolator.remainingDistance + movementPlane.ToPlane(interpolator.position - Parent.GetFeetPosition()).magnitude : 
            float.PositiveInfinity;
        
        public Vector3 SteeringTarget => interpolator.valid ? interpolator.position : FeetPosition;

        public Vector3 FeetPosition => Application.isPlaying ? Parent.GetFeetPosition() : transform.position;
        
        public Vector3 TargetPosition
        {
            get => targetPosition;
            set
            {
                OnTargetChanged?.Invoke();
                targetPosition = value;
                StartFindNewPath();
            }
        }

        void Start()
        {
            seeker = GetComponent<Seeker>();
        }

        private void FixedUpdate()
        {
            if (reachedEndOfPath)
                return;
            
            Vector3 currentPosition = FeetPosition;
            
            interpolator.MoveToCircleIntersection2D(currentPosition, pickNextWaypointDistance, movementPlane);
            Vector2 direction = movementPlane.ToPlane(SteeringTarget - currentPosition);
            
            float distanceToEnd = direction.magnitude + Mathf.Max(0, interpolator.remainingDistance);
            
            reachedEndOfPath = distanceToEnd <= endReachedDistance && interpolator.valid;
            if (reachedEndOfPath)
            {
                TargetReached();
                return;
            }
            
            float slowdown;
            if (interpolator.valid) 
                slowdown = distanceToEnd < slowdownDistance? Mathf.Sqrt (distanceToEnd / slowdownDistance) : 1;
            else 
                slowdown = 1;
            
            Parent.SetSpeedFactor(slowdown);
            Parent.SetMovementDirection(direction.normalized);
        }

        private void OnPathCompleted(Path newPath)
        {
            ABPath p = newPath as ABPath;

            if (p == null) throw new System.Exception("This function only handles ABPaths, do not use special path types");
            
            p.Claim(this);

            if (p.error) {
                p.Release(this);
                return;
            }

            path?.Release(this);

            path = p;

            if (path.vectorPath.Count == 1) path.vectorPath.Add(path.vectorPath[0]);
            interpolator.SetPath(path.vectorPath);

            var graph = path.path.Count > 0 ? AstarData.GetGraph(path.path[0]) as ITransformedGraph : null;
            movementPlane = graph != null ? graph.transform : GraphTransform.identityTransform;

            reachedEndOfPath = false;

            Vector3 position = FeetPosition;
            interpolator.MoveToLocallyClosestPoint((position + p.originalStartPoint) * 0.5f);
            interpolator.MoveToLocallyClosestPoint(position);

            interpolator.MoveToCircleIntersection2D(position, pickNextWaypointDistance, movementPlane);
            
            var distanceToEnd = RemainingDistance;
            if (distanceToEnd <= endReachedDistance) {
                reachedEndOfPath = true;
                TargetReached();
            }
        }

        private void StartFindNewPath()
        {
            seeker.StartPath(transform.position, TargetPosition, OnPathCompleted);
        }

        private void TargetReached()
        {
            Parent.SetSpeedFactor(1f);
            Parent.SetMovementDirection(Vector2.zero);
            OnTargetReached?.Invoke();
        }

        public void ForceEndPathing()
        {
            if(reachedEndOfPath == true)
                return;
            
            targetPosition = Parent.GetFeetPosition();
            reachedEndOfPath = true;
            TargetReached();
        }

        public void SubscribeOnTargetReached(Action callback)
        {
            OnTargetReached += callback;
        }

        public void UnsubscribeTargetReached(Action callback)
        {
            OnTargetReached -= callback;
        }

        public void SubscribeOnTargetChanged(Action callback)
        {
            OnTargetChanged += callback;
        }

        public void UnsubscribeTargetChanged(Action callback)
        {
            OnTargetChanged -= callback;
        }

        private void OnDrawGizmosSelected()
        {
            Draw.Gizmos.Line(FeetPosition, SteeringTarget, Color.white);
            Draw.Gizmos.CircleXZ(FeetPosition, pickNextWaypointDistance, Color.white);
            Draw.Gizmos.CircleXZ(FeetPosition, slowdownDistance, Color.yellow);
            Draw.Gizmos.CircleXZ(FeetPosition, endReachedDistance, Color.red);
        }
    }
}
