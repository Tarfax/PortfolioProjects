using System.Collections.Generic;
using UnityEngine;

public class SignalGateAND : SignalSender {

    [Header("Gate Settings")]
    [SerializeField] private bool revereseSignal;
    [SerializeField] private List<SignalBridge> signalBridges;

    [Header("Debug")]
    [SerializeField] private bool currentSignal;
    [SerializeField] private bool isAllActive;

    private void OnEnable() {
        foreach (SignalBridge bridge in signalBridges) {
            bridge.RegisterOnSignalChanged(OnSignalChanged);
        }
    }

    private void OnDisable() {
        foreach (SignalBridge bridge in signalBridges) {
            bridge.UnregisterOnSignalChanged(OnSignalChanged);
        }
    }

    private void OnSignalChanged() {
        isAllActive = true;
        foreach (SignalBridge bridge in signalBridges) {
            if (bridge.Signal == false) {
                isAllActive = false;
                break;
            }
        }
        currentSignal = isAllActive != revereseSignal;
        SendSignal(currentSignal);
    }

}
