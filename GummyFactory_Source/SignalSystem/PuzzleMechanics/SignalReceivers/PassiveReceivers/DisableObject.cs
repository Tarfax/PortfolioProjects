using System.Collections.Generic;
using UnityEngine;

public class DisableObject : SignalReceiver {

    [Header("On Green Signal")]
    [SerializeField] private List<GameObject> itemsToDisable;

    [Header("Debug")]
    [SerializeField] private bool isActivated;

    protected override bool OnReceiveSignal(bool newSignal) {
        if (isActivated == false && newSignal == true) {
            isActivated = true;
            DisableObjects();
        }
        return newSignal;
    }

    private void DisableObjects() {
        foreach (GameObject obj in itemsToDisable) {
            obj.SetActive(false);
        }
    }

}