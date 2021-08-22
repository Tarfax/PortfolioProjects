using System;
using UnityEngine;

public abstract class SignalReceiver : MonoBehaviour {

    [Header("Signal Receiver Info")] 
    [SerializeField] protected bool signal;
    public bool Signal => signal;
    private bool previousSignal;
    private Action<bool> onSignalReceieved;

    public void ReceiveSignal(bool newSignal) {
        signal = newSignal = OnReceiveSignal(newSignal);
        if (onSignalReceieved != null) {
            onSignalReceieved(newSignal);
        }
    }

    protected abstract bool OnReceiveSignal(bool newSignal);

    public void RegisterOnSignalReceived(Action<bool> callbackFunc) {
        onSignalReceieved += callbackFunc;
    }

    public void UnregisterOnSignalReceived(Action<bool> callbackFunc) {
        onSignalReceieved -= callbackFunc;
    }

    private void OnValidate() {
        if (previousSignal != signal) {
            ReceiveSignal(signal);
            previousSignal = signal;
        }
    }

}
