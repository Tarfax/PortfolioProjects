using _Game.Scripts.Actors;
using UnityEngine;
using UnityEngine.Events;
using UnityEngine.SocialPlatforms;

namespace _Game.Scripts.Systems.RailConveyor
{
    [RequireComponent(typeof(Collider), typeof(Rigidbody))]
    public class Grabber : MonoBehaviour
    {
        [SerializeField, Range(0f, 360f)] private float twistRange = 90f;
        [SerializeField, Range(0f, 180f)] private float twistContactDistance = 5f;
        [SerializeField, Range(0f, 5f)] private float twistSpring = 1f;
        [SerializeField, Range(0f, 5f)] private float twistDamper = 1f;
        [SerializeField, Range(0f, 360f)] private float swingXRange = 90f;
        [SerializeField, Range(0f, 360f)] private float swingZRange = 90f;
        [SerializeField, Range(0f, 180f)] private float swingContactDistance = 5f;
        [SerializeField, Range(0f, 5f)] private float swingSpring = 1f;
        [SerializeField, Range(0f, 5f)] private float swingDamper = 1f;
        [SerializeField] private UnityEvent onGrabbed;
        
        private bool isGrabbing;
        private bool tryGrab = true;
        
        private new Transform transform;
        private Rigidbody body;
        private Joint connectedJoint;

        public bool IsGrabbing => isGrabbing;

        public bool TryGrab
        {
            get => tryGrab;
            set
            {
                if(isGrabbing)
                    tryGrab = false;

                tryGrab = value;
            }
        }

        private void Awake()
        {
            transform = GetComponent<Transform>();
            body = GetComponent<Rigidbody>();
        }

        private void Grab(Collision other)
        {
            connectedJoint = CreateJoint(other);
            isGrabbing = true;
            tryGrab = false;

            if (other.gameObject.TryGetComponent(out Actor actor))
            {
                actor.Rotate = false;
                actor.GetComponent<Rigidbody>().freezeRotation = false;
                actor.ForceEndPathing();
            }

            if (other.gameObject.TryGetComponent(out OnDisableEvent disableEvent) == false)
                disableEvent = other.gameObject.AddComponent<OnDisableEvent>();
            
            disableEvent.SubscribeToEvent(Drop);
            onGrabbed?.Invoke();
        }

        public void Drop()
        {
            if(isGrabbing == false)
                return;

            if (connectedJoint.gameObject.TryGetComponent(out Actor actor))
            {
                actor.Rotate = true;
                actor.GetComponent<Rigidbody>().freezeRotation = true;
            }
            
            connectedJoint.gameObject.GetComponent<OnDisableEvent>().UnsubscribeToEvent(Drop);
            Destroy(connectedJoint);
            isGrabbing = false;
        }

        public void SubscribeToOnGrabbed(UnityAction callback)
        {
            onGrabbed ??= new UnityEvent();
            
            onGrabbed.AddListener(callback);
        }

        public void UnsubscribeToOnGrabbed(UnityAction callback)
        {
            onGrabbed ??= new UnityEvent();

            onGrabbed.RemoveListener(callback);
        }

        private Joint CreateJoint(Collision other)
        {
            CharacterJoint joint = other.rigidbody.gameObject.AddComponent<CharacterJoint>();
            joint.connectedBody = body;
            joint.anchor = other.transform.InverseTransformPoint(other.GetContact(0).point);
            joint.axis = other.transform.InverseTransformDirection(-transform.up);
            joint.swingAxis = other.transform.InverseTransformDirection(-transform.right);

            joint.twistLimitSpring = new SoftJointLimitSpring {spring = twistSpring, damper = twistDamper};
            joint.lowTwistLimit = new SoftJointLimit {limit = -twistRange / 2, contactDistance = twistContactDistance};
            joint.highTwistLimit = new SoftJointLimit {limit = twistRange / 2, contactDistance = twistContactDistance};

            joint.swingLimitSpring = new SoftJointLimitSpring {spring = swingSpring, damper = swingDamper};
            joint.swing1Limit = new SoftJointLimit {limit = swingXRange, contactDistance = swingContactDistance};
            joint.swing2Limit = new SoftJointLimit {limit = swingZRange, contactDistance = swingContactDistance};

            return joint;
        }

        private void OnCollisionEnter(Collision other)
        {
            if (tryGrab == true && isGrabbing == false)
            {
                Grab(other);
            }
        }
    }
}
