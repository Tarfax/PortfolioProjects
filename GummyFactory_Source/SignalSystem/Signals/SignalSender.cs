using _Game.Scripts.Actors;
using System;
using UnityEngine;

public abstract class SignalSender : MonoBehaviour {

    [Header("Signal Sender Info")]
    [SerializeField] protected bool signal;
    private Action<bool> onSignalChanged;

    protected void SendSignal(bool newSignal) {
        signal = newSignal;
        if (onSignalChanged != null) {
            onSignalChanged(newSignal);
        }
    }

    public void RegisterOnSignalChanged(Action<bool> callbackFunc) {
        onSignalChanged += callbackFunc;
    }

    public void UnregisterOnSignalChanged(Action<bool> callbackFunc) {
        onSignalChanged -= callbackFunc;
    }

    public void Interact(Actor actor) {
        OnInteract(actor);
    }

    protected virtual void OnInteract(Actor actor) { }
    

}
