using UnityEngine;

namespace _Game.Scripts.Systems.Spline
{
    public class SplineWalker : MonoBehaviour
    {
        [SerializeField] private float duration;
        [SerializeField] private float lookAhead = 2f;
        [SerializeField] private bool lookForward = true;
        [SerializeField] private bool lockXZRotation = true;
        [SerializeField] private bool useFixedUpdate = true;
        [SerializeField] private SplineWalkerMode mode;
        [SerializeField] private LayerMask lookAheadLayer = -1;
        [SerializeField] private BezierSpline spline;

        private float progress;
        private bool goingForward = true;

        private float timeOfLastRay;
        private const float TimeBetweenRays = 0.05f;
        private bool rayHit;

        private new Transform transform;

        public float Duration
        {
            get => duration;
            set => duration = value;
        }

        public SplineWalkerMode Mode
        {
            get => mode;
            set => mode = value;
        }

        public bool IsWalking { get; set; } = false;

        public BezierSpline Spline
        {
            get => spline;
            set
            { 
                if(spline == value)
                    return;
                spline = value;
                RestartWalk();
            }
        }

        private void Awake()
        {
            transform = GetComponent<Transform>();
            if (spline != null)
                RestartWalk();
        }

        private void Update()
        {
            if(!useFixedUpdate && IsWalking)
                WalkUpdate();
        }

        private void FixedUpdate()
        {
            if(useFixedUpdate && IsWalking)
                WalkUpdate();
        }
        
        public void RestartWalk()
        {
            transform.position = spline.GetPoint(0);
            transform.LookAt(transform.position + spline.GetDirection(0));
            IsWalking = true;
        }

        private void WalkUpdate()
        {
            if(lookAhead > 0)
            {
                if (Time.time >= timeOfLastRay + TimeBetweenRays)
                { 
                    if(Physics.Raycast(transform.position, transform.forward, lookAhead, lookAheadLayer,
                        QueryTriggerInteraction.Ignore))
                    {
                        timeOfLastRay = Time.time;
                        rayHit = true;
                    }
                    else
                        rayHit = false;
                }
                if(rayHit)
                    return;
            }
            
            if (goingForward)
            {
                progress += Time.deltaTime / duration;
                if (progress > 1f)
                {
                    if (mode == SplineWalkerMode.Once)
                        progress = 1f;
                    else if (mode == SplineWalkerMode.Loop)
                        progress -= 1f;
                    else if (mode == SplineWalkerMode.Destroy)
                        Destroy(gameObject);
                    else
                    {
                        progress = 2f - progress;
                        goingForward = false;
                    }
                }
            }
            else
            {
                progress -= Time.deltaTime / duration;
                if (progress < 0f)
                {
                    progress = -progress;
                    goingForward = true;
                }
            }

            Vector3 position = spline.GetPoint(progress);
            transform.localPosition = position;
            if (lookForward)
            {
                Vector3 direction = spline.GetDirection(progress);
                if (lockXZRotation)
                {
                    direction.y = 0;
                    direction.Normalize();
                }
                transform.LookAt(position + direction);
            }
        }
    }
}