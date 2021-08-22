using UnityEngine;
using UnityEngine.Events;

namespace _Game.Scripts.Systems
{
    public class OnDisableEvent : MonoBehaviour
    {
        [SerializeField] private UnityEvent onDisable;

        private void OnDisable()
        {
            onDisable?.Invoke();
        }

        public void SubscribeToEvent(UnityAction callback)
        {
            onDisable ??= new UnityEvent();

            onDisable.AddListener(callback);
        }

        public void UnsubscribeToEvent(UnityAction callback)
        {
            onDisable ??= new UnityEvent();
            
            onDisable.RemoveListener(callback);
        }
    }
}