using UnityEngine;

namespace _Game.Scripts.Systems
{
    public class Elevator : MonoBehaviour
    {
        private Animator animator;

        private int isDownHash = Animator.StringToHash("isDown");

        private void Awake()
        {
            animator = GetComponent<Animator>();
        }

        public void TriggerElevator()
        {
            bool currentValue = animator.GetBool(isDownHash);
            animator.SetBool(isDownHash, !currentValue);
        }

        public void SetElevator(bool isDown)
        {
            animator.SetBool(isDownHash, isDown);
        }
    }
}
