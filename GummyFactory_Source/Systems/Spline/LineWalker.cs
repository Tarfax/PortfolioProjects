using System;
using UnityEngine;

namespace _Game.Scripts.Systems.Spline
{
    public class LineWalker : MonoBehaviour
    {
        [SerializeField] private float maxSpeed = 5f;
        [SerializeField] private float acceleration = 10f;
        [SerializeField] private float lookAhead = 0.5f;
        [SerializeField] private bool useFixedUpdate = true;
        [SerializeField] private EndBehaviourType endBehaviour = EndBehaviourType.Destroy;
        [SerializeField] private LayerMask lookAheadLayer = -1;
        [SerializeField] private Line line;

        private float distance;
        private Vector3 direction;
        private Vector3 velocity;

        private const float TimeBetweenRays = 0.05f;
        private float timeOfLastRay = 0f;
        private bool rayHit = false;

        private new Transform transform;

        public bool IsWalking { get; set; }

        public EndBehaviourType EndBehaviour
        {
            get => endBehaviour;
            set => endBehaviour = value;
        }

        public Line Line
        {
            get => line;
            set
            {
                if(line == value)
                    return;
                line = value;
                RestartWalk();
            }
        }

        private void Awake()
        {
            transform = GetComponent<Transform>();
            if(line != null)
                RestartWalk();
        }

        private void Update()
        {
            if (useFixedUpdate == true || IsWalking == false)
                return;
            
            Walk();
        }

        private void FixedUpdate()
        {
            if(useFixedUpdate == false || IsWalking == false)
                return;
            
            Walk();
        }

        public void RestartWalk()
        {
            transform.position = line.WorldPoint0;
            direction = (line.WorldPoint1 - line.WorldPoint0).normalized;
            transform.LookAt(transform.position + direction);
            distance = Vector3.Distance(line.WorldPoint0, line.WorldPoint1);
            IsWalking = true;
        }


        private void Walk()
        {
            if(lookAhead > 0)
            {
                if (Time.time >= timeOfLastRay + TimeBetweenRays)
                {
                    if (Physics.Raycast(transform.position, direction, lookAhead, lookAheadLayer,
                        QueryTriggerInteraction.Ignore))
                    {
                        timeOfLastRay = Time.time;
                        rayHit = true;
                    }
                    else
                        rayHit = false;
                }
                if (rayHit)
                    return;
            }
            
            velocity = Vector3.MoveTowards(velocity, direction * maxSpeed, acceleration * Time.deltaTime);
            transform.position += velocity * Time.deltaTime;
            if (Vector3.Distance(transform.position, line.WorldPoint0) >= distance)
                ReachedEnd();
        }

        private void ReachedEnd()
        {
            switch (endBehaviour)
            {
                case EndBehaviourType.Stop:
                    IsWalking = false;
                    break;
                case EndBehaviourType.Loop:
                    RestartWalk();
                    break;
                case EndBehaviourType.Destroy:
                    Destroy(gameObject);
                    break;
            }
        }

        public enum EndBehaviourType
        {
            Stop,
            Loop,
            Destroy
        }
    }
}