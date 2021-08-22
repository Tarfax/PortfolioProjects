using UnityEngine;

namespace _Game.Scripts.Systems.Sound
{
    public class StopBGM : MonoBehaviour
    {
        [SerializeField] private bool fade;
        [SerializeField, Range(0, 5f)] private float fadeDuration;
        [Space(10)]
        [SerializeField] private bool stopOnAwake;

        private void Awake()
        {
            if(stopOnAwake)
                Stop();
        }

        public void Stop()
        {
            SoundManager.StopBGM(fade, fadeDuration);
        }
    }
}