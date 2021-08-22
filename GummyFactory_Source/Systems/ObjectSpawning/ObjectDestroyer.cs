using UnityEngine;

namespace _Game.Scripts.Systems.ObjectSpawning
{
    public class ObjectDestroyer : MonoBehaviour
    {
        private void OnTriggerEnter(Collider other)
        {
            Transform objectTransform = other.transform;
            Transform parentTransform = objectTransform.parent;
            while (parentTransform != null)
            {
                objectTransform = parentTransform;
                parentTransform = objectTransform.parent;
            }

            Destroy(objectTransform.gameObject);
        }
    }
}