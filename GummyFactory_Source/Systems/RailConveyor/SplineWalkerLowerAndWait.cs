using _Game.Scripts.Systems.Spline;
using UnityEngine;
using UnityEngine.Events;

namespace _Game.Scripts.Systems.RailConveyor
{
    public class SplineWalkerLowerAndWait : MonoBehaviour
    {
        [SerializeField, Tooltip("Values under 0 will revert to default.")]
        private float grabRange = -1f;
        [SerializeField] private UnityEvent onWalkerArrived;
        [SerializeField] private bool detectOccupiedWalkers = false;
        private void GrabSequenceComplete(SplineWalker walker, GrabberLowerer lowerer)
        {
            walker.IsWalking = true;
            lowerer.UnsubscribeToOnGrabSequenceComplete(() => GrabSequenceComplete(walker, lowerer));
        }
        
        private void OnTriggerEnter(Collider other)
        {
            if (other.TryGetComponent(out SplineWalker walker))
            {
                GrabberLowerer lowerer = other.GetComponentInChildren<GrabberLowerer>();

                if (lowerer != null && (detectOccupiedWalkers || !lowerer.IsGrabbing))
                {
                    onWalkerArrived?.Invoke();

                    walker.IsWalking = false;
                    lowerer.LowerAndWaitTillGrabbed(grabRange);
                    lowerer.SubscribeToOnGrabSequenceComplete(() => GrabSequenceComplete(walker, lowerer));
                }
            }
        }
    }
}