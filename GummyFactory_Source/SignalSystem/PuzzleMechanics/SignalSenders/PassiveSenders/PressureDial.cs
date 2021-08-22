using _Game.Scripts.Systems.Sound;
using UnityEngine;

public class PressureDial : Dial {

    [Header("Dial Needle Settings", order = 2)]
    [Range(0f, 100f)] [SerializeField] protected float needleSpeed = 60f;
    [Range(0f, 100f)] [SerializeField] private float needleGainAmount = 25f;
    [Header("Sound")]
    [SerializeField] private SoundDefinition pressureDialSound;


    [Header("Debug")]
    [SerializeField] private float currentAngle;
    [SerializeField] private float rangeMin = -0.17f;
    [SerializeField] private float rangeMax = 0.15f;

    private float timer;
    private float timerTimer;
    private bool currentSignal;
    private bool lastSignal;

    protected override void OnStart() {
        if (pressureDialSound.AudioClips[0] != null) {
            timer = pressureDialSound.AudioClips[0].length;
        }
        timerTimer = timer;
        isActivated = false;
        currentSignal = false;
        lastSignal = false;
    }

    protected override void OnUpdate() {
        float randomFloat = Random.Range(rangeMin, rangeMax);
        angle -= needleSpeed * (Time.deltaTime/* + randomFloat*/);
        if (angle > angleMax) {
            angle = angleMax;
        }
        if (angle <= 0f) {
            angle = 0f;
        }

        dialNeedle.transform.eulerAngles = new Vector3(0, 0, GetRotation(angle, dialEndAngle, dialStartAngle));
        currentSignal = angle > Mathf.Abs(intervalAngle) && angle < (Mathf.Abs(intervalAngle) + fillAmount);
        if (currentSignal != lastSignal) {
            TestNeedle();
            lastSignal = currentSignal;
        }
        currentAngle = angle;

        if (isActivated == true) {
            timerTimer -= Time.deltaTime;
            if (timerTimer <= 0f) {
                SoundManager.PlaySFX(pressureDialSound.AudioClips[0], Vector3.zero, pressureDialSound.VolumeMultiplier, pressureDialSound.PitchMultiplier);
                timerTimer = timer;
            }
        }
    }

    private void TestNeedle() {
        if (currentSignal == true) {
            indicator.sprite = indicatorGreen;
            SendSignal(true);
        }
        else {
            indicator.sprite = indicatorRed;
            SendSignal(false);
        }
    }

    protected override void OnButtonDown() {
        dialButton.sprite = dialButtonPressed;
        angle += needleGainAmount;
        SoundManager.PlaySFX(pressureDialSound.AudioClipsGreenSignal[0], Vector3.zero, pressureDialSound.VolumeMultiplier, pressureDialSound.PitchMultiplier);
    }

    protected override void OnButtonUp() {
        dialButton.sprite = dialButtonNormal;
    }



}
