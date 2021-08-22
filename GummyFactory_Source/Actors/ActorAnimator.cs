using System;
using System.Collections;
using UnityEngine;
using Random = UnityEngine.Random;

namespace _Game.Scripts.Actors
{
    [RequireComponent(typeof(Animator))]
    public class ActorAnimator : ActorComponentBase
    {
        [SerializeField, Range(0, 1f)] private float idleBlendIntensity = 1f;
        [SerializeField] private float blendTime = 0.5f;
        [SerializeField] private Vector2 timeBetweenSwitchesRange = new Vector2(1, 5);
        [SerializeField] private float blendValue;

        private float timeToSwitch;

        private Animator animator;

        private event Action onInteractFinished;

        private int idleBlendHash = Animator.StringToHash("IdleBlend");
        private int walkHash = Animator.StringToHash("isWalking");
        private int punchingHash = Animator.StringToHash("isPunching");
        private int buttonPushingHash = Animator.StringToHash("isButtonPushing");
        private int kicking1Hash = Animator.StringToHash("isKicking1");
        private int kicking2Hash = Animator.StringToHash("isKicking2");
        private int pickingUpObjectHash = Animator.StringToHash("isPickingUpObject");
        private int liftingHash = Animator.StringToHash("isLifting");
        private int pushingHash = Animator.StringToHash("isPushing");
        private int pointingHash = Animator.StringToHash("isPointing");

        public Animation CurrentAnimation { get; private set; } = Animation.Idle;

        private void Awake()
        {
            animator = GetComponent<Animator>();
            StartCoroutine(LerpBlendIdle(Random.Range(0f, 1f)));
            //timeToSwitch = Time.time + Random.Range(timeBetweenSwitchesRange.x, timeBetweenSwitchesRange.y);
        }

        private void Update()
        {
            /*if (timeToSwitch <= Time.time)
            {
                StopAllCoroutines();
                StartCoroutine(LerpBlendIdle(Random.Range(0f, 1f)));
                timeToSwitch = Time.time + Random.Range(timeBetweenSwitchesRange.x, timeBetweenSwitchesRange.y);
            }*/
        }

        private IEnumerator LerpBlendIdle(float newValue)
        {
            float t = 0;
            float initialValue = blendValue;
            while (blendValue != newValue)
            {
                t += blendTime != 0 ? Time.deltaTime / blendTime : 1;
                blendValue = Mathf.Lerp(initialValue, newValue, t);
                animator.SetFloat(idleBlendHash, blendValue);
                yield return null;
            }
        }

        public bool GetAnimationState(Animation animation)
        {
            switch (animation)
            {
                case Animation.Idle:
                    return true;
                case Animation.Walking:
                    return animator.GetBool(walkHash);
                case Animation.Punching:
                    return animator.GetBool(punchingHash);
                case Animation.ButtonPushing:
                    return animator.GetBool(buttonPushingHash);
                case Animation.Kicking1:
                    return animator.GetBool(kicking1Hash);
                case Animation.Kicking2:
                    return animator.GetBool(kicking2Hash);
                case Animation.PickingUpObject:
                    return animator.GetBool(pickingUpObjectHash);
                case Animation.Lifting:
                    return animator.GetBool(liftingHash);
                case Animation.Pushing:
                    return animator.GetBool(pushingHash);
                case Animation.Pointing:
                    return animator.GetBool(pointingHash);
                default:
                    return false;
            }
        }

        public void SetAnimationState(Animation animation, bool state)
        {
            CurrentAnimation = animation;
            switch (animation)
            {
                case Animation.Walking:
                    animator.SetBool(walkHash, state);
                    break;
                case Animation.Punching:
                    animator.SetBool(punchingHash, state);
                    break;
                case Animation.ButtonPushing:
                    animator.SetBool(buttonPushingHash, state);
                    break;
                case Animation.Kicking1:
                    animator.SetBool(kicking1Hash, state);
                    break;
                case Animation.Kicking2:
                    animator.SetBool(kicking2Hash, state);
                    break;
                case Animation.PickingUpObject:
                    animator.SetBool(pickingUpObjectHash, state);
                    break;
                case Animation.Lifting:
                    animator.SetBool(liftingHash, state);
                    break;
                case Animation.Pushing:
                    animator.SetBool(pushingHash, state);
                    break;
                case Animation.Pointing:
                    animator.SetBool(pointingHash, state);
                    break;
            }
        }

        public void InvokeAnimationStateFinished(AnimationCategory category)
        {
            switch (category)
            {
                case AnimationCategory.Interacting:
                    onInteractFinished?.Invoke();
                    break;
            }
        }

        public void SubscribeToOnAnimationStateFinished(AnimationCategory category, Action action)
        {
            switch (category)
            {
                case AnimationCategory.Interacting:
                    onInteractFinished += action;
                    break;
            }
        }

        public void UnsubscribeToOnAnimationStateFinished(AnimationCategory category, Action action)
        {
            switch (category)
            {
                case AnimationCategory.Interacting:
                    onInteractFinished -= action;
                    break;
            }
        }

        public static AnimationCategory AnimationToCategory(Animation animation)
        {
            switch (animation)
            {
                case Animation.Walking:
                    return AnimationCategory.Walking;
                case Animation.Punching:
                    return AnimationCategory.Interacting;
                case Animation.ButtonPushing:
                    return AnimationCategory.Interacting;
                case Animation.Kicking1:
                    return AnimationCategory.Interacting;
                case Animation.Kicking2:
                    return AnimationCategory.Interacting;
                case Animation.PickingUpObject:
                    return AnimationCategory.Interacting;
                case Animation.Lifting:
                    return AnimationCategory.Interacting;
                case Animation.Pushing:
                    return AnimationCategory.Interacting;
                case Animation.Pointing:
                    return AnimationCategory.Interacting;
                default:
                    return AnimationCategory.Idle;
            }
        }

        public enum Animation
        {
            Idle,
            Walking,
            Punching,
            ButtonPushing,
            Kicking1,
            Kicking2,
            PickingUpObject,
            Lifting,
            Pushing,
            Pointing
        }

        public enum AnimationCategory
        {
            Idle,
            Walking,
            Interacting
        }
    }
}