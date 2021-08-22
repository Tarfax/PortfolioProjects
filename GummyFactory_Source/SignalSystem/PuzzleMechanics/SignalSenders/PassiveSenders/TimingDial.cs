using _Game.Scripts.Actors;
using _Game.Scripts.Systems.Sound;
using UnityEngine;

public class TimingDial : Dial {

    [Header("Dial Needle Settings", order = 2)]
    [Range(0f, 100f)] [SerializeField] private float needleSpeed = 30f;
    private bool angleIsCorrect;

    [Header("Sound", order = 3)]
    [SerializeField] private SoundDefinition timingDialSound;

    private float timer;
    private float timerTimer;

    protected override void OnStart() {
        if (timingDialSound.AudioClips[0] != null) {
            timer = timingDialSound.AudioClips[0].length;
        }
        timerTimer = timer;
        isActivated = false;
    }

    protected override void OnUpdate() {
        angle += needleSpeed * Time.deltaTime;
        if (angle > angleMax) {
            angle = 0f;
        }
        dialNeedle.transform.eulerAngles = new Vector3(0, 0, GetRotation(angle, dialEndAngle, dialStartAngle));
        if (angle > Mathf.Abs(intervalAngle) && angle < (Mathf.Abs(intervalAngle) + fillAmount)) {
            angleIsCorrect = true;
            indicator.sprite = indicatorGreen;
        }
        else {
            angleIsCorrect = false;
            indicator.sprite = indicatorRed;
        }

        if (isActivated == true) {
            timerTimer -= Time.deltaTime;
            if (timerTimer <= 0f) {
                if (timingDialSound.AudioClips[0] != null) {
                    SoundManager.PlaySFX(timingDialSound.AudioClips[0], Vector3.zero, timingDialSound.VolumeMultiplier, timingDialSound.PitchMultiplier);
                }
                timerTimer = timer;
            }
        }
    }

    private void TestNeedle() {
        if (angleIsCorrect == true) {
            needleSpeed = 0f;
            SendSignal(true);
            SoundManager.PlaySFX(timingDialSound.AudioClipsGreenSignal[0], Vector3.zero, timingDialSound.VolumeMultiplier, timingDialSound.PitchMultiplier);
        }
        else {
            angle = 0f;
            SendSignal(false);
            SoundManager.PlaySFX(timingDialSound.AudioClipsRedSignal[0], Vector3.zero, timingDialSound.VolumeMultiplier, timingDialSound.PitchMultiplier);
        }
    }

    protected override void OnButtonDown() {
        dialButton.sprite = dialButtonPressed;
    }

    protected override void OnButtonUp() {
        dialButton.sprite = dialButtonNormal;
        TestNeedle();
    }

}



