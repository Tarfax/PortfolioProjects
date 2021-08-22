using _Game.Scripts.Actors;
using MC_Utility.EventSystem;
using UnityEngine;

public class ActivateUI : SignalReceiver {

    [Header("UI To Toggle")]
    [SerializeField] private Dial UIToActivate;
    [SerializeField] private Side side;

    [Header("Settings")]
    [SerializeField] private bool needGreenSignalToActivate;
    [SerializeField] InteractableDefinition notAvailableText;

    private Actor actor;

    [Header("Debug")]
    [SerializeField] private bool debugMode;
    [SerializeField] private bool isActivated;
    [SerializeField] private bool canInteract;

    private void OnEnable() {
        EventSystem<UseDebugEvent>.RegisterListener(UseDebugEvent);
    }

    private void OnDisable() {
        EventSystem<UseDebugEvent>.UnregisterListener(UseDebugEvent);
    }

    private void Start() {
        if (needGreenSignalToActivate == true) {
            canInteract = false;
        }
        else {
            canInteract = true;
        }
    }

    private void Update() {
        if (isActivated == true && debugMode == false && canInteract == false) {
            InteractFinished();
        }
    }

    private void OnMouseDown() {
        if (debugMode == true) {
            if (isActivated == false) {
                Interact(null);
            }
            else if (isActivated == true) {
                InteractFinished();
            }
        }
    }

    public void Interact(Actor actor) {
        if (isActivated == true || (canInteract == false || debugMode == true)) {
            return;
        }

        if (actor != null) {
            actor.SubscribeOnTargetChanged(InteractFinished);
            this.actor = actor;
        }

        UIToActivate.SetActive(true);
        UIToActivate.SetSide(side);
        isActivated = true;

        ReceiveSignal(isActivated);
    }

    public void InteractFinished() {
        UIToActivate.SetActive(false);
        isActivated = false;
        if (actor != null) {
            actor.UnsubscribeTargetChanged(InteractFinished);
        }
    }
    
    protected override bool OnReceiveSignal(bool newSignal) {
        canInteract = newSignal;
        return newSignal;
    }

    private void UseDebugEvent(UseDebugEvent debugEvent) {
        debugMode = debugEvent.Toggle;
    }

}
