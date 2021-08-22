using UnityEngine;

namespace _Game.Scripts.Actors
{
    [RequireComponent(typeof(Rigidbody))]
    public class MovementController : ActorComponentBase
    {
        [SerializeField, Range(0, 200)] private float maxSpeed = 10f;
        [SerializeField, Range(0, 200)] private float maxAcceleration = 10f;
        [SerializeField, Range(0f, 90f)] private float maxSlopeAngle = 25f;
        [SerializeField, Range(0f, 2f)] private float snapProbeDistance = 0.2f;
        [SerializeField] private LayerMask probeMask = -1;

        private Rigidbody rigidbody;
        private Rigidbody connectedBody;
        private Rigidbody previoudConnectedBody;

        private float speedFactor = 1f;
        private Vector2 moveDirection;
        private Vector3 velocity;
        private Vector3 connectionVelocity;
        private Vector3 connectionWorldPosition;
        private Vector3 connectionLocalPosition;
        
        private float minSlopeDotProduct;

        private int groundContactCount;
        private int steepContactCount;
        private Vector3 contactNormal;
        private Vector3 steepNormal;

        private int stepsSinceLastGrounded;

        private bool IsGrounded => groundContactCount > 0;
        private bool IsSteepGrounded => steepContactCount > 0;


        public Vector2 MoveDirection
        {
            set
            {
                value.Normalize();
                moveDirection = value;
                Parent.SetAnimationState(ActorAnimator.Animation.Walking, moveDirection != Vector2.zero);
            }
        }

        public float SpeedFactor {
            set => speedFactor = Mathf.Clamp01(value);
        }

        public Vector3 ContactNormal => contactNormal;

        private void Awake()
        {
            rigidbody = GetComponent<Rigidbody>();
            OnValidate();
        }

        private void OnValidate()
        {
            minSlopeDotProduct = Mathf.Cos(maxSlopeAngle * Mathf.Deg2Rad);
        }

        private void FixedUpdate()
        {
            UpdateState();
            if(IsGrounded)
                AdjustVelocity();

            rigidbody.velocity = velocity;
            ClearState();
        }

        private void OnCollisionEnter(Collision collision)
        {
            EvaluateCollision(collision);
        }

        private void OnCollisionStay(Collision collision)
        {
            EvaluateCollision(collision);
        }

        private void EvaluateCollision(Collision collision)
        {
            for (int i = 0; i < collision.contactCount; i++)
            {
                Vector3 normal = collision.GetContact(i).normal;
                if (normal.y >= minSlopeDotProduct)
                {
                    groundContactCount++;
                    contactNormal += normal;
                    connectedBody = collision.rigidbody;
                }
                else if (normal.y > -0.1f)
                {
                    steepContactCount++;
                    steepNormal += normal;
                    if (groundContactCount == 0)
                        connectedBody = collision.rigidbody;
                }
            }
        }

        private void UpdateState()
        {
            stepsSinceLastGrounded++;
            velocity = rigidbody.velocity;
            if (IsGrounded || SnapToGround() || CheckSteepContacts())
            {
                stepsSinceLastGrounded = 0;
                if(groundContactCount > 1)
                    contactNormal.Normalize();
            }
            else
                contactNormal = Vector3.up;
            if(connectedBody)
                if(connectedBody.isKinematic || connectedBody.mass >= rigidbody.mass)
                    UpdateConnectionState();
        }

        private void ClearState()
        {
            groundContactCount = steepContactCount = 0;
            contactNormal = steepNormal = Vector3.zero;
            connectionVelocity = Vector3.zero;
            previoudConnectedBody = connectedBody;
            connectedBody = null;
        }

        private void AdjustVelocity()
        {
            Vector3 xAxis = ProjectOnContactPlane(Vector3.right).normalized;
            Vector3 zAxis = ProjectOnContactPlane(Vector3.forward).normalized;

            Vector3 relativeVelocity = velocity - connectionVelocity;
            
            Vector3 adjustedVelocity = Vector3.zero;
            adjustedVelocity.x = moveDirection.x * maxSpeed * speedFactor - Vector3.Dot(relativeVelocity, xAxis);
            adjustedVelocity.z = moveDirection.y * maxSpeed * speedFactor - Vector3.Dot(relativeVelocity, zAxis);

            adjustedVelocity = 
                Vector3.ClampMagnitude(adjustedVelocity, maxAcceleration * Time.deltaTime);

            velocity += xAxis * adjustedVelocity.x + zAxis * adjustedVelocity.z;
        }

        private void UpdateConnectionState()
        {
            if(connectedBody == previoudConnectedBody)
            {
                Vector3 connectionMovement = connectedBody.transform.TransformPoint(connectionLocalPosition) -
                                             connectionWorldPosition;
                connectionVelocity = connectionMovement / Time.deltaTime;
            }
            connectionWorldPosition = rigidbody.position;
            connectionLocalPosition = connectedBody.transform.InverseTransformPoint(connectionWorldPosition);
        }

        private bool SnapToGround()
        {
            if (stepsSinceLastGrounded > 1)
                return false;
            if (!Physics.Raycast(
                Parent.GetFeetPosition(), Vector3.down, out RaycastHit hit, snapProbeDistance, probeMask))
                return false;
            if (hit.normal.y < minSlopeDotProduct)
                return false;

            groundContactCount = 1;
            contactNormal = hit.normal;
            float speed = velocity.magnitude;
            float dot = Vector3.Dot(velocity, contactNormal);
            if (dot > 0) 
                velocity = (velocity - contactNormal * dot).normalized * speed;
            connectedBody = hit.rigidbody;
            return true;
        }

        private bool CheckSteepContacts()
        {
            if (steepContactCount > 1)
            {
                steepNormal.Normalize();
                if (steepNormal.y >= minSlopeDotProduct)
                {
                    groundContactCount = 1;
                    contactNormal = steepNormal;
                    return true;
                }
            }

            return false;
        }

        private Vector3 ProjectOnContactPlane(Vector3 vector)
        {
            return vector - contactNormal * Vector3.Dot(vector, contactNormal);
        }
    }
}
