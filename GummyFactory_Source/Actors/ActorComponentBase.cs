using UnityEngine;

namespace _Game.Scripts.Actors
{
    public abstract class ActorComponentBase : MonoBehaviour
    {
        public Actor Parent { get; set; }
    }
}
