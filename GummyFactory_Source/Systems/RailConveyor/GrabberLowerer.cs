using System.Collections;
using UnityEngine;
using UnityEngine.Events;

namespace _Game.Scripts.Systems.RailConveyor
{
    [RequireComponent(typeof(Grabber))]
    public class GrabberLowerer : MonoBehaviour
    {
        [SerializeField] private float speed;
        [SerializeField] private float grabRange = 10f;
        [SerializeField] private float rayRadius = 0.125f;
        [SerializeField] private LayerMask rayHitMask = -1;
        [SerializeField] private Vector3 rayStartOffset;
        [SerializeField] private UnityEvent onGrabSequenceComplete;
        
        private bool isGrabSequenceRunning;
        private bool waitingTillGrabbed;

        private new Transform transform;
        private Grabber grabber;

        public float GrabRange
        {
            get => grabRange;
            set => grabRange = value;
        }
        public bool IsGrabbing => grabber.IsGrabbing;

        private void Awake()
        {
            transform = GetComponent<Transform>();
            grabber = GetComponent<Grabber>();
            grabber.TryGrab = false;
        }

        public bool TryLowerAndGrab(float range = -1f)
        {
            if(isGrabSequenceRunning || grabber.IsGrabbing)
                return false;

            range = range >= 0 ? range : grabRange;
            if (CheckForGrabbable(out RaycastHit hitInfo, range))
            {
                StartCoroutine(GrabSequence(hitInfo, range));
                return true;
            }

            return false;
        }

        public void TryLowerTillGrabbed(float range = -1f)
        {
            range = range >= 0 ? range : grabRange;
            StartCoroutine(RepeatGrabSequenceTillGrabbed(range));
        }

        public void LowerAndWaitTillGrabbed(float range = -1f)
        {
            range = range >= 0 ? range : grabRange;
            StartCoroutine(LowerAndWaitSequence(range));
        }

        private bool CheckForGrabbable(out RaycastHit hitInfo, float distance)
        {
            Vector3 direction = -transform.up;
            Ray ray = new Ray(transform.position + rayStartOffset, direction);

            if (Physics.SphereCast(ray, rayRadius, out hitInfo, distance, rayHitMask))
            {
                if (hitInfo.rigidbody != null)
                {
                    return true;
                }
            }
            return false;
        }

        private bool CheckForGround(out RaycastHit hitInfo, float distance)
        {
            Vector3 direction = -transform.up;
            Ray ray = new Ray(transform.position + rayStartOffset, direction);

            if (Physics.SphereCast(ray, rayRadius, out hitInfo, distance, rayHitMask))
            {
                return true;
            }
            return false;
        }

        private IEnumerator RepeatGrabSequenceTillGrabbed(float range)
        {
            waitingTillGrabbed = true;
            WaitForSeconds delayBetween = new WaitForSeconds(0.5f);
            
            while (grabber.IsGrabbing == false)
            {
                if (isGrabSequenceRunning == false)
                    TryLowerAndGrab();

                yield return delayBetween;
            }
        }
        
        private IEnumerator GrabSequence(RaycastHit hitInfo, float range)
        {
            isGrabSequenceRunning = true;
            grabber.TryGrab = true;

            Vector3 initialPosition = transform.position;
            Vector3 hitPosition = hitInfo.point;
            hitPosition.y += rayRadius;
            Vector3 grabPosition = initialPosition;
            float distance = Vector3.Distance(initialPosition, hitPosition);
            float smoothTime = distance / speed;
            Vector3 refVelocity = Vector3.zero;

            while (!grabber.IsGrabbing)
            {
                transform.position = Vector3.SmoothDamp(transform.position, hitPosition, ref refVelocity, smoothTime);
                grabPosition = transform.position;
                if(Vector3.Distance(grabPosition, hitPosition) <= 0.01f)
                {
                    if (CheckForGrabbable(out hitInfo, range - Mathf.Abs(initialPosition.y - grabPosition.y)))
                    {
                        hitPosition = hitInfo.point;
                        hitPosition.y += rayRadius;
                        distance = Vector3.Distance(initialPosition, hitPosition);
                        smoothTime = distance / speed;
                    }
                    else 
                        break;
                }
                yield return null;
            }

            distance = Vector3.Distance(grabPosition, initialPosition);
            smoothTime = distance / speed;
            
            while (transform.position != initialPosition)
            {
                transform.position = Vector3.SmoothDamp(transform.position, initialPosition, ref refVelocity, smoothTime);
                if (Vector3.Distance(transform.position, initialPosition) <= 0.01f)
                    transform.position = initialPosition;
                yield return null;
            }
            if(waitingTillGrabbed == false)
                onGrabSequenceComplete?.Invoke();
            else if (IsGrabbing)
            {
                waitingTillGrabbed = false;
                onGrabSequenceComplete?.Invoke();
            }

            isGrabSequenceRunning = false;
        }

        private IEnumerator LowerAndWaitSequence(float range)
        {
            isGrabSequenceRunning = true;
            grabber.TryGrab = true;

            Vector3 initialPosition = transform.position;
            
            Vector3 hitPosition;
            if (CheckForGround(out RaycastHit hitInfo, range))
                hitPosition = hitInfo.point;
            else
                hitPosition = initialPosition - transform.up * range;

            hitPosition.y += rayRadius;
            Vector3 grabPosition = initialPosition;
            float distance = Vector3.Distance(initialPosition, hitPosition);
            float smoothTime = distance / speed;
            Vector3 refVelocity = Vector3.zero;

            while (grabber.IsGrabbing == false && grabPosition != hitPosition)
            {
                transform.position = Vector3.SmoothDamp(transform.position, hitPosition, ref refVelocity, smoothTime);
                grabPosition = transform.position;
                if(Vector3.Distance(grabPosition, hitPosition) <= 0.01f)
                {
                    if (CheckForGround(out hitInfo, range - Mathf.Abs(initialPosition.y - grabPosition.y)))
                        hitPosition = hitInfo.point;
                    else
                        hitPosition = initialPosition - transform.up * range;
                    
                    hitPosition.y += rayRadius; 
                    distance = Vector3.Distance(initialPosition, hitPosition);
                    smoothTime = distance / speed;
                }
                yield return null;
            }

            while (grabber.IsGrabbing == false)
            {
                yield return null;
            }
            
            distance = Vector3.Distance(grabPosition, initialPosition);
            smoothTime = distance / speed;
            
            while (transform.position != initialPosition)
            {
                transform.position = Vector3.SmoothDamp(transform.position, initialPosition, ref refVelocity, smoothTime);
                if (Vector3.Distance(transform.position, initialPosition) <= 0.01f)
                    transform.position = initialPosition;
                yield return null;
            }
            if(waitingTillGrabbed == false)
                onGrabSequenceComplete?.Invoke();
            else if (IsGrabbing)
            {
                waitingTillGrabbed = false;
                onGrabSequenceComplete?.Invoke();
            }

            isGrabSequenceRunning = false;
        }

        public void SubscribeToOnGrabSequenceComplete(UnityAction callback)
        {
            onGrabSequenceComplete ??= new UnityEvent();

            onGrabSequenceComplete.AddListener(callback);
        }

        public void UnsubscribeToOnGrabSequenceComplete(UnityAction callback)
        {
            onGrabSequenceComplete ??= new UnityEvent();

            onGrabSequenceComplete.RemoveListener(callback);
        }
        
        private void OnDrawGizmosSelected()
        {
            Gizmos.color = Color.red;
            Transform trs = gameObject.transform;
            Vector3 position = trs.position + rayStartOffset;
            Gizmos.DrawLine(position, position + -trs.up * grabRange);
        }
    }
}