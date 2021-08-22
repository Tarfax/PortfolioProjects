using System;
using System.Collections.Generic;
using UnityEngine;

namespace _Game.Scripts.Systems
{
    public class PhysicsConveyorBelt : MonoBehaviour
    {
        [SerializeField] private float speed = 5f;
        
        [SerializeField, Tooltip("Higher values will stop objects moving around as much.")]
        private float correctionAmount = 5f;
        
        [SerializeField] private ConveyorAxis axis;
        [SerializeField] private bool isInverted;

        private new Transform transform;
        
        private HashSet<Rigidbody> connectedBodies;

        private void Awake()
        {
            transform = GetComponent<Transform>();
            connectedBodies = new HashSet<Rigidbody>();
        }

        private void FixedUpdate()
        {
            Vector3 direction = GetDirection();
            Vector3 desiredVelocity = direction * speed;
            float correctionChange = correctionAmount * Time.deltaTime;
            
            foreach (Rigidbody body in connectedBodies)
            {
                Vector3 velocity = body.velocity;
                Vector3 projectedVelocity = Vector3.Project(velocity, direction);

                velocity -= projectedVelocity;
                
                projectedVelocity = Vector3.MoveTowards(projectedVelocity, desiredVelocity, 
                    float.PositiveInfinity);
                velocity = Vector3.MoveTowards(velocity, desiredVelocity - projectedVelocity, correctionChange);
                
                velocity += projectedVelocity;

                body.velocity = velocity;
            }
        }

        private Vector3 GetDirection()
        {
            switch (axis)
            {
                case ConveyorAxis.X:
                    return isInverted ? -transform.right : transform.right;
                case ConveyorAxis.Z:
                    return isInverted ? -transform.forward : transform.forward;
                case ConveyorAxis.Y:
                    return isInverted ? -transform.up : transform.up;
            }

            return Vector3.zero;
        }

        private void OnCollisionEnter(Collision other)
        {
            if (connectedBodies.Contains(other.rigidbody) == false)
                AddBody(other.rigidbody);
        }

        private void OnCollisionExit(Collision other)
        {
            if (connectedBodies.Contains(other.rigidbody) == true)
                RemoveBody(other.rigidbody);
        }

        private void AddBody(Rigidbody body)
        {
            if (body.gameObject.TryGetComponent(out OnDisableEvent disableEvent) == false)
                disableEvent = body.gameObject.AddComponent<OnDisableEvent>();
            
            disableEvent.SubscribeToEvent(() => RemoveBody(body));
            connectedBodies.Add(body);
        }

        private void RemoveBody(Rigidbody body)
        {
            if(body.gameObject.TryGetComponent(out OnDisableEvent disableEvent))
                disableEvent.UnsubscribeToEvent(() => RemoveBody(body));

            connectedBodies.Remove(body);
        }
        
        private enum ConveyorAxis
        {
            X,
            Z,
            Y
        }
    }
}
