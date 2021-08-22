using System.Collections.Generic;
using UnityEngine;

public class SignalIF : MonoBehaviour {

    [Header("Listen For Signal From")]
    [SerializeField] private SignalSender listenTo;
    [SerializeField] private bool listenForSignal = true;

    [Header("Input")]
    [SerializeField] private SignalSender signalSender;

    [Header("Output")]
    [SerializeField] private List<SignalReceiver> signalReceivers;

    [Header("Settings")]
    [SerializeField] private bool signalToSend = true;
    [SerializeField] private float signalDelay = 0f;
    private float signalDelayTimer = 0f;

    [Header("Debug")]
    [SerializeField] private bool currentSignal;
    private bool hasNewSignal;
    [SerializeField] private bool isActive;

    private void OnEnable() {
        listenTo.RegisterOnSignalChanged(ListenToSignalReception);
        signalSender.RegisterOnSignalChanged(OnSignalChanged);
    }

    private void OnDisable() {
        listenTo.UnregisterOnSignalChanged(ListenToSignalReception);
        signalSender.UnregisterOnSignalChanged(OnSignalChanged);
    }

    private void Update() {
        if (hasNewSignal == true && signalDelay > 0f) {
            signalDelayTimer -= Time.deltaTime;
            if (signalDelayTimer <= 0f) {
                SendSignal();
                hasNewSignal = false;
            }
        }
    }

    private void ListenToSignalReception(bool signal) {
        if (signal == listenTo) {
            isActive = true;
            return;
        }
        isActive = false;
    }

    private void OnSignalChanged(bool signal) {
        if (hasNewSignal == true || isActive == false) {
            return;
        }


        if (signalDelay > 0f) {
            hasNewSignal = true;
            signalDelayTimer = signalDelay;
        }
        else {
            SendSignal();
        }
    }

    private void SendSignal() {
        foreach (SignalReceiver item in signalReceivers) {
            item.ReceiveSignal(signalToSend);
        }
    }

}
