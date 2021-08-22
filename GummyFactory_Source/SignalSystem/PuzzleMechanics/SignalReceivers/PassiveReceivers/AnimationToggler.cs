using UnityEngine;

public class AnimationToggler : SignalReceiver {

    [Header("Settings")]
    [SerializeField] private Animator animator;

    [Header("Debug")]
    [SerializeField] private bool isActive;
    [SerializeField] private bool isPlaying;
    private float timer = 0.2f;
    private float lengthTimer;

    protected override bool OnReceiveSignal(bool newSignal) {
        animator.SetTrigger("Run");
        isPlaying = true;
        isActive = true;
        lengthTimer = timer;
        return newSignal;
    }

    void Update() {
        if (isPlaying == true) {
            lengthTimer -= Time.deltaTime;
            if (lengthTimer <= 0f) {
                isPlaying = false;
                isActive = false;
            }
        }
    }

}
