using _Game.Scripts.Actors;
using MC_Utility.EventSystem;
using UnityEngine;

public class SimpleButton : SignalSender {

    [Header("Button Settings")]
    [SerializeField] private bool useOneTime;
    [SerializeField] private bool signalToSend = true;

    [Header("Debug")]
    private bool debugMode;
    private bool isActivated;

    private void OnEnable() {
        EventSystem<UseDebugEvent>.RegisterListener(UseDebugEvent);
    }

    private void OnDisable() {
        EventSystem<UseDebugEvent>.UnregisterListener(UseDebugEvent);
    }

    private void OnMouseDown() {
        if (debugMode == true) {
            Interact(null);
            
        }
    }

    protected override void OnInteract(Actor actor) {
        if (isActivated == false) {
            if (useOneTime == true) {
                isActivated = true;
            }
            SendSignal(signalToSend);
        }
    }

    private void UseDebugEvent(UseDebugEvent debugEvent) {
        debugMode = debugEvent.Toggle;
    }

}


