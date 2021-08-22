using _Game.Scripts.Actors;
using MC_Utility.EventSystem;
using System.Collections.Generic;
using UnityEngine;

public class Item : SignalReceiver {

    [Header("Settings")]
    [SerializeField] private ItemDefinition item;
    [SerializeField] private InventorySide inventorySide;
    [Space]
    [SerializeField] private bool needGreenSignal = true;
    [Space]
    [SerializeField] private bool infinite = false;
    [SerializeField] private bool disableObjectOnUse = true;
    [Space]
    [SerializeField] private List<SignalReceiver> objectsToSignalWhenPickedUp;
    //[Header("Interactive Text")]
    //[SerializeField] private bool useInteractiveText;
    //[SerializeField] private InteractableDefinition interactiveText;
    //[SerializeField] private float heightOffset;

    //[Header("References")]
    //[SerializeField] private MeshRenderer meshRenderer;
    //private Material outlineMaterial;
    //private Material defaultMaterial;

    [Header("Debug")]
    [SerializeField] private bool debugMode;
    [SerializeField] private bool isActivated;
    private bool isInteractable = true;

    private void OnEnable() {
        EventSystem<UseDebugEvent>.RegisterListener(UseDebugEvent);
    }

    private void OnDisable() {
        EventSystem<UseDebugEvent>.UnregisterListener(UseDebugEvent);
    }

    private void Start() {
        if (needGreenSignal == true) {
            isInteractable = false;
        }
    }

    private void OnMouseDown() {
        if (debugMode == true) {
            Interact(null);
        }
    }

    public void Interact(Actor actor) {
        if (isActivated == false && isInteractable == true) {
            UI_InventoryController.PickupItem(item, inventorySide);
            EventSystem<PickupItemEvent>.FireEvent(PickupItemEvent.GetEvent(item));
            if (infinite == false) {
                isActivated = true;
                if (disableObjectOnUse == true) {
                    gameObject.SetActive(false);
                }
                foreach (SignalReceiver receiver in objectsToSignalWhenPickedUp) {
                    receiver.ReceiveSignal(false);
                }
            }
        }
    }

    private void UseDebugEvent(UseDebugEvent debugEvent) {
        debugMode = debugEvent.Toggle;
    }

    protected override bool OnReceiveSignal(bool newSignal) {
        if (newSignal == true) {
            isInteractable = true;
        }
        else {
            if (needGreenSignal == true) {
                isInteractable = false;
            }
        }
        return newSignal;
    }
}

public class PickupItemEvent : IEvent {
    public ItemDefinition item;

    public static PickupItemEvent GetEvent(ItemDefinition item) {
        return new PickupItemEvent() { item = item };
    }
}