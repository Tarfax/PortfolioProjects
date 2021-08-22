using UnityEngine;

namespace _Game.Scripts.Systems.RailConveyor
{
    public class RobotArmController : MonoBehaviour
    {
        private Grabber grabber;
        private Animator animator;

        private int isGrabbingHash = Animator.StringToHash("isGrabbing");

        private void Awake()
        {
            grabber = GetComponentInChildren<Grabber>();
            animator = GetComponent<Animator>();
        }

        private void OnEnable()
        {
            grabber.SubscribeToOnGrabbed(OnGrabbedCallback);
        }

        private void OnDisable()
        {
            grabber.UnsubscribeToOnGrabbed(OnGrabbedCallback);
        }

        public void Drop()
        {
            grabber.Drop();
            animator.SetBool(isGrabbingHash, false);
        }

        public void ReadyGrab()
        {
            grabber.TryGrab = true;
        }

        private void OnGrabbedCallback()
        {
            animator.SetBool(isGrabbingHash, true);
        }
    }
}