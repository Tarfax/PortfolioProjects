using System.Collections.Generic;
using UnityEngine;

public class ToggleObject : SignalReceiver {

    [Header("Objects To Toggle")]
    [SerializeField] private List<GameObject> itemsToToggle;
    [SerializeField] private bool startEnabled = true;

    [Header("Debug")]
    [SerializeField] private bool isActivated;

    private void Start() {
        OnReceiveSignal(startEnabled);
    }

    protected override bool OnReceiveSignal(bool newSignal) {

        if (newSignal == false) {
            isActivated = false;
            DisableObjects();
        }
        else {
            isActivated = true;
            EnableObjects();
        }

        return newSignal;
    }

    private void DisableObjects() {
        foreach (GameObject obj in itemsToToggle) {
            obj.SetActive(false);
        }
    }

    private void EnableObjects() {
        foreach (GameObject obj in itemsToToggle) {
            obj.SetActive(true);
        }
    }

}
