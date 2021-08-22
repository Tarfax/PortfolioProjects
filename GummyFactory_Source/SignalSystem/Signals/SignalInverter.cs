using UnityEngine;

public class SignalInverter : SignalSender {

    [Header("Listen to receiver")]
    [SerializeField] private SignalReceiver signalReceiver;

    [Header("Settings")]
    [SerializeField] private bool invertSignal = true;
    [SerializeField] private float signalDelay = 0f;
    private float signalDelayTimer = 0f;

    [Header("Debug")]
    [SerializeField] private bool currentSignal;
    private bool hasNewSignal;

    private void OnEnable() {
        if (signalReceiver != null) {
            signalReceiver.RegisterOnSignalReceived(OnSignalChanged);
        }
    }

    private void OnDisable() {
        if (signalReceiver != null) {
            signalReceiver.UnregisterOnSignalReceived(OnSignalChanged);
        }
    }

    private void Update() {
        if (hasNewSignal == true && signalDelay > 0f) {
            signalDelayTimer -= Time.deltaTime;
            if (signalDelayTimer <= 0f) {
                SendSignal(currentSignal);
                hasNewSignal = false;
            }
        }
    }

    private void OnSignalChanged(bool signal) {
        if (hasNewSignal == true) {
            return;
        }

        if (invertSignal == true) {
            currentSignal = !signal;
        }
        else {
            currentSignal = signal;
        }

        if (signalDelay > 0f) {
            hasNewSignal = true;
            signalDelayTimer = signalDelay;
        }
        else {
            SendSignal(currentSignal);
        }
    }

}
