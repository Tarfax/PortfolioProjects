using UnityEngine;

namespace _Game.Scripts.Systems.Sound
{
    public class PlayBGM : MonoBehaviour
    {
        [SerializeField] private AudioClip clip;
        [SerializeField] private bool fade;
        [SerializeField, Range(0, 5f)] private float fadeDuration;
        [Space(10)]
        [SerializeField] private bool playOnAwake;

        private void Awake()
        {
            if(playOnAwake)
                Play();
        }

        public void Play()
        {
            SoundManager.PlayBGM(clip, fade, fadeDuration);
        }
    }
}
