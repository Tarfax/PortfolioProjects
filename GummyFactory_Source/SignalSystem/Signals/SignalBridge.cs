using System;
using System.Collections.Generic;
using UnityEngine;

public class SignalBridge : MonoBehaviour {

    [Header("Input")]
    [SerializeField] private SignalSender signalSender;

    [Header("Output")]
    [SerializeField] private List<SignalReceiver> signalReceivers;

    [Header("Brigde Settings")]
    [SerializeField] private bool revereseSignal;
    [SerializeField] private float signalDelay = 0f;
    private float signalDelayTimer = 0f;

    private Action OnSignalChanged;

    [Header("Debug")]
    [SerializeField] private bool receivedSignal;
    public bool Signal { get => receivedSignal; }
    private bool hasNewSignal;

    private void OnEnable() {
        signalSender.RegisterOnSignalChanged(ChangeSignal);
        hasNewSignal = false;
    }

    private void OnDisable() {
        signalSender.UnregisterOnSignalChanged(ChangeSignal);
    }

    private void Update() {
        if (hasNewSignal == true) {
            signalDelayTimer -= Time.deltaTime;
            if (signalDelayTimer <= 0f) {

                foreach (SignalReceiver item in signalReceivers) {
                    item.ReceiveSignal(receivedSignal);
                }

                if (OnSignalChanged != null) {
                    OnSignalChanged();
                }

                //SendSignal();
                hasNewSignal = false;
            }
        }
    }

    private void ChangeSignal(bool newSignal) {
        receivedSignal = newSignal;
        if (revereseSignal == true) {
            receivedSignal = !receivedSignal;
        }

        if (signalDelay > 0f) {
            hasNewSignal = true;
            signalDelayTimer = signalDelay;
            return;
        }

        foreach (SignalReceiver item in signalReceivers) {
            item.ReceiveSignal(receivedSignal);
        }

        if (OnSignalChanged != null) {
            OnSignalChanged();
        }
    }

    public void RegisterOnSignalChanged(Action callbackFunc) {
        OnSignalChanged += callbackFunc;
    }

    public void UnregisterOnSignalChanged(Action callbackFunc) {
        OnSignalChanged -= callbackFunc;
    }

}