using _Game.Scripts.Actors;
using MC_Utility.EventSystem;
using UnityEngine;

public class ToggleButton : SignalSender {

    [Header("Settings")]
    [SerializeField] private bool startSignal;
    [SerializeField] private bool useOneTime;

    [Header("Debug")]
    private bool debugMode;
    private bool isActivated;
    [SerializeField] private bool currentSignal;

    private void OnEnable() {
        EventSystem<UseDebugEvent>.RegisterListener(UseDebugEvent);
    }

    private void OnDisable() {
        EventSystem<UseDebugEvent>.UnregisterListener(UseDebugEvent);
    }

    private void Start() {
        currentSignal = startSignal;
    }


    private void OnMouseDown() {
        if (debugMode == true) {
            Toggle();
        }
    }

    protected override void OnInteract(Actor actor) {
        if (isActivated == false) {
            if (useOneTime == true) {
                isActivated = true;
            }
            Toggle();
        }
    }

    private void Toggle() {
        currentSignal = !currentSignal;
        SendSignal(currentSignal);
    }

    private void UseDebugEvent(UseDebugEvent debugEvent) {
        debugMode = debugEvent.Toggle;
    }

}


