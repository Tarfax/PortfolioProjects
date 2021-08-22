using System;
using System.Collections;
using _Game.Scripts.Systems.Interactables;
using UnityEngine;

namespace _Game.Scripts.Actors
{
    public class Interactor : ActorComponentBase
    {
        private bool isWaitingToInteract;
        private static bool isInteractingGlobal;

        private Interactable endOfAnimationInteractable;
        private Action onInteractableReachedAction;
        private static event Action OnInteractionDoneGlobal;
        public Interactable TargetInteractable { get; set; }

        public Action OnInteractableReachedAction => onInteractableReachedAction;


        private void Awake()
        {
            onInteractableReachedAction = InteractOnInteractableReached;
        }

        private void Start()
        {
            Parent.SubscribeOnTargetChanged(OnTargetChanged);
        }

        private void InteractOnInteractableReached()
        {
            if(CheckIfInPosition() == false)
                return;
            
            Parent.UnsubscribeTargetReached(onInteractableReachedAction);
            if (Parent.HasRotator)
                StartCoroutine(InteractWhenRotated(TargetInteractable));
            else
                Interact(TargetInteractable);
        }

        public void Interact(Interactable interactable)
        {
            if (isInteractingGlobal == true)
            {
                StartCoroutine(RetryInteractTillAvailable(interactable));
                return;
            }

            endOfAnimationInteractable = interactable;
            isInteractingGlobal = true;
            Parent.SetIsBusy(true, this);
            interactable.StartInteraction(Parent);
            
            switch (interactable.SelectedWaitMode)
            {
                case Interactable.WaitMode.None:
                    interactable.FinishInteraction(Parent);
                    isInteractingGlobal = false;
                    Parent.SetIsBusy(false, this);
                    break;
                case Interactable.WaitMode.Time:
                    Parent.SetAnimationState(interactable.Animation, true);
                    StartCoroutine(TimeDelayedInteract(interactable, interactable.WaitTime));
                    break;
                case Interactable.WaitMode.Animation:
                    endOfAnimationInteractable = interactable;
                    Parent.SetAnimationState(interactable.Animation, true);
                    Parent.SubscribeToOnAnimationStateFinished(
                        ActorAnimator.AnimationToCategory(interactable.Animation),
                        OnAnimationFinishedInteract
                        );
                    break;
            }
        }

        private bool CheckIfInPosition()
        {
            Vector3 position = Parent.GetFeetPosition();
            Vector3 targetPosition = TargetInteractable.GetInteractionPosition();
            bool targetReached = Math.Abs(position.x - targetPosition.x) < 0.15f &&
                                 Math.Abs(position.z - targetPosition.z) < 0.15f;

            return targetReached;
        }

        private IEnumerator InteractWhenRotated(Interactable interactable)
        {
            Parent.SetFacingDirection(interactable.GetInteractionDirection());
            while (Vector3.Dot(Parent.GetFacingDirection(), interactable.GetInteractionDirection()) < 0.99f)
            {
                yield return null;
            }

            Interact(interactable);
        }

        private IEnumerator RetryInteractTillAvailable(Interactable interactable)
        {
            while (isInteractingGlobal == true)
            {
                yield return null;
            }

            Interact(interactable);
        }

        private IEnumerator TimeDelayedInteract(Interactable interactable, float delay)
        {
            yield return new WaitForSeconds(delay);
            interactable.FinishInteraction(Parent);
            isInteractingGlobal = false;
            Parent.SetAnimationState(interactable.Animation, false);
            Parent.SetIsBusy(false, this);
        }

        private void OnAnimationFinishedInteract()
        {
            endOfAnimationInteractable.FinishInteraction(Parent);
            isInteractingGlobal = false;
            Parent.UnsubscribeToOnAnimationStateFinished(
                ActorAnimator.AnimationToCategory(endOfAnimationInteractable.Animation), 
                OnAnimationFinishedInteract
                );
            Parent.SetAnimationState(endOfAnimationInteractable.Animation, false);
            Parent.SetIsBusy(false, this);
        }

        private void OnTargetChanged()
        {
            StopAllCoroutines();
            Parent.UnsubscribeTargetReached(InteractOnInteractableReached);
        }
    }
}