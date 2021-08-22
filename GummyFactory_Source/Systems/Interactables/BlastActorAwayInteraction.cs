using _Game.Scripts.Actors;
using UnityEngine;

namespace _Game.Scripts.Systems.Interactables
{
    public class BlastActorAwayInteraction : MonoBehaviour
    {
        public void BlastHimAway(Actor actor)
        {
            Rigidbody rigidbody = actor.GetComponent<Rigidbody>();

            rigidbody.AddForce((Vector3.left + Vector3.up) * 100, ForceMode.Impulse);
        }
    }
}