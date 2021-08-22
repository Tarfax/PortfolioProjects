using System.Collections;
using _Game.Scripts.Systems.Spline;
using UnityEngine;
using UnityEngine.Events;

namespace _Game.Scripts.Systems.RailConveyor
{
    public class LineWalkerStopAndDrop : MonoBehaviour
    {
        [SerializeField] private float waitBeforeDrop = 0.25f;
        [SerializeField] private float waitAfterDrop = 0.25f;
        [SerializeField] private UnityEvent onWalkerArrived;

        private void OnTriggerEnter(Collider other)
        {
            if (other.TryGetComponent(out LineWalker walker))
            {
                Grabber grabber = other.GetComponentInChildren<Grabber>();
                onWalkerArrived?.Invoke();
                if (grabber != null)
                {
                    StartCoroutine(DropSequence(walker, grabber));
                }
            }
        }

        private IEnumerator DropSequence(LineWalker walker, Grabber grabber)
        {
            walker.IsWalking = false;
            yield return new WaitForSeconds(waitBeforeDrop);
            grabber.Drop();
            yield return new WaitForSeconds(waitAfterDrop);
            walker.IsWalking = true;
        }
    }
}