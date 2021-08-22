using System.Collections.Generic;
using UnityEngine;

public class EnableItem : SignalReceiver {

    [Header("On Green Signal")]
    [SerializeField] private List<GameObject> itemsToEnable;

    [Header("Debug")]
    [SerializeField] private bool isActivated;

    void Start() {
        DisableObjects();
    }

    protected override bool OnReceiveSignal(bool newSignal) {
        if (isActivated == false && newSignal == true) {
            isActivated = true;
            EnableObjects();
        }
        else {
            isActivated = false;
            DisableObjects();
        }
        return newSignal;
    }

    private void EnableObjects() {
        foreach (GameObject obj in itemsToEnable) {
            obj.SetActive(true);
        }
    }

    private void DisableObjects() {
        foreach (GameObject obj in itemsToEnable) {
            obj.SetActive(false);
        }
    }

}
