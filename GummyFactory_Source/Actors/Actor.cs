using System;
using System.Collections.Generic;
using _Game.Scripts.Systems.Interactables;
using UnityEngine;
using Object = UnityEngine.Object;

namespace _Game.Scripts.Actors
{
    [SelectionBase]
    public class Actor : MonoBehaviour
    {
        private bool hasMovementController;
        private MovementController movementController;
        private bool hasRotator;
        private Rotator rotator;
        private bool hasPathFollower;
        private PathFollower pathFollower;
        private bool hasInteractor;
        private Interactor interactor;
        private bool hasAnimator;
        private ActorAnimator actorAnimator;

        private new Transform transform;

        private HashSet<int> busyIDs = new HashSet<int>();

        public bool HasMovementController => hasMovementController;
        public bool HasRotator => hasRotator;
        public bool HasPathFollower => hasPathFollower;
        public bool HasInteractor => hasInteractor;
        public bool HasAnimator => hasAnimator;

        public bool IsBusy
        {
            get => busyIDs.Count > 0;
        }

        public bool Rotate
        {
            get => hasRotator && rotator.Rotate;
            set
            {
                if (hasRotator)
                    rotator.Rotate = value;
            }
        }

        public Vector3 ContactNormal => hasMovementController ? movementController.ContactNormal : Vector3.up;

        private void Awake()
        {
            if (TryGetComponent(out movementController))
            {
                hasMovementController = true;
                movementController.Parent = this;
            }

            if (TryGetComponent(out rotator))
            {
                hasRotator = true;
                rotator.Parent = this;
            }

            if (TryGetComponent(out pathFollower))
            {
                hasPathFollower = true;
                pathFollower.Parent = this;
            }

            if (TryGetComponent(out interactor))
            {
                hasInteractor = true;
                interactor.Parent = this;
            }

            if (TryGetComponent(out actorAnimator))
            {
                hasAnimator = true;
                actorAnimator.Parent = this;
            }

            transform = GetComponent<Transform>();
        }

        public void SetIsBusy(bool value, Object setter)
        {
            int instanceID = setter.GetInstanceID();

            if (value == true)
                busyIDs.Add(instanceID);
            else if (busyIDs.Contains(instanceID))
                busyIDs.Remove(instanceID);
        }

        public Vector3 GetFeetPosition()
        {
            return transform.position;
        }

        public void SetMovementDirection(Vector2 direction)
        {
            if (hasMovementController == false)
                return;

            movementController.MoveDirection = direction;

            if (hasRotator)
                rotator.DesiredFacingDirection = direction;
        }

        public void SetSpeedFactor(float speed)
        {
            if (hasMovementController == false)
                return;

            movementController.SpeedFactor = speed;
        }

        public void SetTargetPosition(Vector3 position)
        {
            if (IsBusy == true || hasPathFollower == false)
                return;
            
            pathFollower.TargetPosition = position;
        }

        public void ForceEndPathing()
        {
            if(hasPathFollower == true)
                pathFollower.ForceEndPathing();
        }

        public void MoveToAndInteract(Interactable interactable)
        {
            if(IsBusy == true)
                return;
            
            if (hasPathFollower == true)
            {
                SetTargetPosition(interactable.GetInteractionPosition());
                if (hasInteractor == true)
                {
                    interactor.TargetInteractable = interactable;
                    pathFollower.SubscribeOnTargetReached(interactor.OnInteractableReachedAction);
                }
            }
        }

        public void SetFacingDirection(Vector2 direction)
        {
            if (hasRotator)
                rotator.DesiredFacingDirection = direction;
        }

        public Vector2 GetFacingDirection()
        {
            if (hasRotator)
                return rotator.CurrentFacingDirection;

            Vector3 direction = transform.forward;
            return new Vector2(direction.x, direction.z);
        }

        public bool GetAnimationState(ActorAnimator.Animation animation)
        {
            if (hasAnimator == false)
                return false;

            return actorAnimator.GetAnimationState(animation);
        }

        public void SetAnimationState(ActorAnimator.Animation animation, bool state)
        {
            if(hasAnimator == false)
                return;

            actorAnimator.SetAnimationState(animation, state);
        }

        public void SubscribeOnTargetReached(Action callback)
        {
            if(hasPathFollower)
                pathFollower.SubscribeOnTargetReached(callback);
        }

        public void UnsubscribeTargetReached(Action callback)
        {
            if (hasPathFollower)
                pathFollower.UnsubscribeTargetReached(callback);
        }
        
        public void SubscribeOnTargetChanged(Action callback)
        {
            if(hasPathFollower)
                pathFollower.SubscribeOnTargetChanged(callback);
        }

        public void UnsubscribeTargetChanged(Action callback)
        {
            if (hasPathFollower)
                pathFollower.UnsubscribeTargetChanged(callback);
        }
        
        public void SubscribeToOnAnimationStateFinished(ActorAnimator.AnimationCategory category, Action action)
        {
            if(hasAnimator == false)
                return;

            actorAnimator.SubscribeToOnAnimationStateFinished(category, action);
        }

        public void UnsubscribeToOnAnimationStateFinished(ActorAnimator.AnimationCategory category, Action action)
        {
            if(hasAnimator == false)
                return;

            actorAnimator.UnsubscribeToOnAnimationStateFinished(category, action);
        }
    }
}
