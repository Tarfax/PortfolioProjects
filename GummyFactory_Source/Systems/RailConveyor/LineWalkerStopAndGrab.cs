using _Game.Scripts.Systems.Spline;
using UnityEngine;
using UnityEngine.Events;

namespace _Game.Scripts.Systems.RailConveyor
{
    public class LineWalkerStopAndGrab : MonoBehaviour
    {
        [SerializeField, Tooltip("Values under 0 will revert to default.")]
        private float grabRange = -1f;
        [SerializeField] private bool waitTillGrabbed;
        [SerializeField] private UnityEvent onWalkerArrived;
        [SerializeField] private bool detectOccupiedWalkers = false;
        private void GrabSequenceComplete(LineWalker walker, GrabberLowerer lowerer)
        {
            walker.IsWalking = true;
            lowerer.UnsubscribeToOnGrabSequenceComplete(() => GrabSequenceComplete(walker, lowerer));
        }
        
        private void OnTriggerEnter(Collider other)
        {
            if (other.TryGetComponent(out LineWalker walker))
            {
                GrabberLowerer lowerer = other.GetComponentInChildren<GrabberLowerer>();
                
                if (lowerer != null && (detectOccupiedWalkers || !lowerer.IsGrabbing))
                {
                    onWalkerArrived?.Invoke();
                    
                    if(waitTillGrabbed)
                    {
                        walker.IsWalking = false;
                        lowerer.TryLowerTillGrabbed(grabRange);
                        lowerer.SubscribeToOnGrabSequenceComplete(() => GrabSequenceComplete(walker, lowerer));
                    }                    
                    else if (lowerer.TryLowerAndGrab(grabRange))
                    {
                        walker.IsWalking = false;
                        lowerer.SubscribeToOnGrabSequenceComplete(() => GrabSequenceComplete(walker, lowerer));
                    }
                }
            }
        }
    }
}