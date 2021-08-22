using _Game.Scripts.Systems.Sound;
using UnityEngine;

namespace _Game.Scripts.Systems.Interactables
{
    public class PlaySoundInteraction : MonoBehaviour
    {
        [SerializeField] private AudioClip clipToPlay;
        [SerializeField] private Transform position;

        private void OnValidate()
        {
            position = GetComponentInChildren<Transform>();
        }

        public void PlaySound()
        {
            SoundManager.PlaySFX(clipToPlay, position.position);
        }
    }
}