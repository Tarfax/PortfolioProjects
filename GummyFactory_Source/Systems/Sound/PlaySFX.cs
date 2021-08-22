using UnityEngine;

namespace _Game.Scripts.Systems.Sound
{
    public class PlaySFX : SignalReceiver
    {
        [SerializeField] private Transform soundOrigin;
        [SerializeField] private SoundDefinition soundDefinition;

        [SerializeField] private float delayUntilPlay;
        private float delayUntilPlayTimer;

        private void OnValidate()
        {
            if (soundOrigin == null)
                soundOrigin = GetComponentInChildren<Transform>();
        }

        private void Update() {
            if (delayUntilPlay > 0f) {
                delayUntilPlayTimer -= Time.deltaTime;
            }
        }

        public void Play()
        {
            if(soundDefinition == null)
                return;
            
            SoundManager.PlaySFX(soundDefinition.AudioClips, soundOrigin.position, soundDefinition.VolumeMultiplier,
                soundDefinition.PitchMultiplier);
        }

        public void PlayRandomized()
        {
            if(soundDefinition == null)
                return;
            
            SoundManager.PlaySFXRandomized(soundDefinition.AudioClips, soundOrigin.position, soundDefinition.VolumeMultiplier);
        }

        public void PlayRandomized(bool newSignal) {
            if (soundDefinition == null) {
                return;
            }

            AudioClip[] audioClips = soundDefinition.AudioClips;

            if (newSignal == false && soundDefinition.IsRedListEmpty() == false) {
                Debug.Log("false");
                audioClips = soundDefinition.AudioClipsRedSignal;
            } else if (newSignal == true && soundDefinition.IsGreenListEmpty() == false) {
                Debug.Log("true");
                audioClips = soundDefinition.AudioClipsGreenSignal;
            }

            if (audioClips == null)
                return;

            if (audioClips.Length == 1)
                SoundManager.PlaySFXRandomized(audioClips[0], soundOrigin.position, soundDefinition.VolumeMultiplier);
            else
                SoundManager.PlaySFXRandomized(audioClips, soundOrigin.position, soundDefinition.VolumeMultiplier);
        }

        protected override bool OnReceiveSignal(bool newSignal) {
            PlayRandomized(newSignal);
            return newSignal;
        }

    }
}