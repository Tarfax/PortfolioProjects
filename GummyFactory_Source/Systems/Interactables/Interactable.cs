using _Game.Scripts.Actors;
using UnityEngine;
using UnityEngine.Events;

namespace _Game.Scripts.Systems.Interactables
{
    public class Interactable : MonoBehaviour
    {
        [SerializeField] private Transform interactionPosition;
        [SerializeField] private WaitMode waitMode = WaitMode.None;
        [SerializeField] private float waitTime = 0.0f;
        [SerializeField] private new ActorAnimator.Animation animation;
        [SerializeField] private ActorUnityEvent onInteractStarted;
        [SerializeField] private ActorUnityEvent onInteractFinished;

        public WaitMode SelectedWaitMode => waitMode;
        public float WaitTime => waitTime;
        public ActorAnimator.Animation Animation => animation;

        private void OnValidate()
        {
            if (interactionPosition == null)
                interactionPosition = GetComponentInChildren<Transform>();
        }

        public Vector3 GetInteractionPosition()
        {
            return interactionPosition.position;
        }

        public Vector2 GetInteractionDirection()
        {
            Vector3 direction = interactionPosition.forward;
            return new Vector2(direction.x, direction.z).normalized;
        }

        public void StartInteraction(Actor actor)
        {
            onInteractStarted.Invoke(actor);
        }
        public void FinishInteraction(Actor actor)
        {
            onInteractFinished.Invoke(actor);
        }

        public void SubscribeToOnInteractStarted(UnityAction<Actor> callback)
        {
            onInteractStarted.AddListener(callback);
        }

        public void UnsubscribeToOnInteractStarted(UnityAction<Actor> callback)
        {
            onInteractStarted.RemoveListener(callback);
        }
        
        public void SubscribeToOnInteractFinished(UnityAction<Actor> callback)
        {
            onInteractFinished.AddListener(callback);
        }

        public void UnsubscribeToOnInteractFinished(UnityAction<Actor> callback)
        {
            onInteractFinished.RemoveListener(callback);
        }

        public enum WaitMode
        {
            None,
            Time,
            Animation
        }
    }
}