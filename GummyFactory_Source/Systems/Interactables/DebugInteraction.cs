using _Game.Scripts.Actors;
using UnityEngine;

namespace _Game.Scripts.Systems.Interactables
{
    public class DebugInteraction : MonoBehaviour
    {
        public void LogInteraction(Actor actor)
        {
            Debug.Log($"{actor.gameObject.name} interacted with {gameObject.name}");
        }
    }
}