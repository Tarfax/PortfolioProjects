using UnityEngine;

namespace _Game.Scripts.Systems.Sound
{
    public class PlaySFXLooping : MonoBehaviour
    {
        [SerializeField] private AudioClip[] clip;
        [SerializeField] private Transform soundOrigin;
        [SerializeField] private float volumeMultiplier = 1f;
        [SerializeField] private float pitchMultiplier = 1f;
        [SerializeField] private bool playOnAwake;

        private int id;
        private bool isPlaying;
        private bool isQuitting;

        private void OnValidate()
        {
            if (soundOrigin == null)
                soundOrigin = GetComponentInChildren<Transform>();
        }

        private void OnEnable()
        {
            if(playOnAwake)
                Play();
        }

        private void OnDisable()
        {
            if(isPlaying && isQuitting == false)
                Stop();
        }

        private void OnApplicationQuit()
        {
            isQuitting = true;
        }

        public void Play()
        {
            if(isPlaying == true)
                return;
            
            if(clip.Length == 1)
                id = SoundManager.PlayLoopingSFX(clip[0], soundOrigin.position, volumeMultiplier, pitchMultiplier);
            else 
                id = SoundManager.PlayLoopingSFX(clip, soundOrigin.position, volumeMultiplier, pitchMultiplier);

            isPlaying = true;
        }

        public void Stop()
        {
            if(isPlaying == false)
                return;

            SoundManager.StopLoopingSFX(id);

            isPlaying = false;
        }
    }
}