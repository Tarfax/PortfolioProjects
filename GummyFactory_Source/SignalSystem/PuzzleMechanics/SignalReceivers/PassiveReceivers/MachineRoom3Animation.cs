using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MachineRoom3Animation : MonoBehaviour {

    [SerializeField] private List<SignalReceiver> redLights;
    [SerializeField] private Animator animator;

    [SerializeField] private int greenLightCount;

    private void OnEnable() {
        foreach (var item in redLights) {
            item.RegisterOnSignalReceived(NewSignalDetected);
        }
    }

    private void OnDisable() {
        foreach (var item in redLights) {
            item.UnregisterOnSignalReceived(NewSignalDetected);
        }
    }

    private void NewSignalDetected(bool newSignal) {
        greenLightCount = 0;
        foreach (var item in redLights) {
            if (item.Signal == true) {
                greenLightCount++;
            }
        }
        UpdateAnimator();
    }

    private void UpdateAnimator() {
        animator.SetInteger("Lamps", greenLightCount);
    }

}
