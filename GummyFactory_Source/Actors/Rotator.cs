using UnityEngine;

namespace _Game.Scripts.Actors
{
    public class Rotator : ActorComponentBase
    {
        [SerializeField] private float rotationSpeed = 5f;
        [SerializeField] private bool rotateWithSlope = false;

        private Vector2 currentDesiredDirection;
        private Quaternion desiredRotation;
        
        private new Transform transform;

        public bool Rotate { get; set; } = true;

        public Vector2 DesiredFacingDirection
        {
            get => currentDesiredDirection;
            set
            {
                if(value == Vector2.zero)
                    return;
                currentDesiredDirection = value;
                desiredRotation = Quaternion.LookRotation(new Vector3(value.x, 0, value.y));
            }
        }

        public Vector2 CurrentFacingDirection => new Vector2(transform.forward.x, transform.forward.z);

        private void Awake()
        {
            transform = GetComponent<Transform>();
        }

        private void Update()
        {
            if(Rotate == false)
                return;
            
            float maxDegreeChange = rotationSpeed * Time.deltaTime;
            transform.rotation = Quaternion.RotateTowards(transform.rotation, desiredRotation, maxDegreeChange);
        }
    }
}