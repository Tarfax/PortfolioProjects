using _Game.Scripts.Actors;
using _Game.Scripts.Player;
using _Game.Scripts.Systems.Interactables;
using MC_Utility.EventSystem;
using UnityEngine;

public class UseItemOn : SignalSender {

    [Header("Accepted Item")]
    [SerializeField] private ItemDefinition itemToAccept;

    [Header("Settings")]
    [SerializeField] private InteractableDefinition ifNoInteractable;
    [SerializeField] private InteractableDefinition whenRightInteractable;

    //[Header("References")]
    //[SerializeField] private MeshRenderer meshRenderer;
    //private Material outlineMaterial;
    //private Material defaultMaterial;

    [Header("Debug")]
    [SerializeField] private bool debugMode;
    [SerializeField] private bool isActivated;
    [SerializeField] private bool canBeInteracted;
    [SerializeField] private bool canDragAndDropItem;
    [SerializeField] private bool isHovered;

    private UI_ItemSlot itemSlot;

    private Actor actor;

    private void OnEnable() {
        EventSystem<ItemSlotOnBeginDragEvent>.RegisterListener(OnBeginDragItemEvent);
        EventSystem<ItemSlotOnEndDragEvent>.RegisterListener(OnEndDragItemEvent);
        EventSystem<UseDebugEvent>.RegisterListener(UseDebugEvent);
    }

    private void OnDisable() {
        EventSystem<ItemSlotOnBeginDragEvent>.UnregisterListener(OnBeginDragItemEvent);
        EventSystem<ItemSlotOnEndDragEvent>.UnregisterListener(OnEndDragItemEvent);
        EventSystem<UseDebugEvent>.UnregisterListener(UseDebugEvent);
    }

    private void OnMouseEnter() {
        if (canDragAndDropItem == true) {
            UI_TooltipController.ShowHoveredInteractable(whenRightInteractable, transform);
        }
        else {
            UI_TooltipController.ShowHoveredInteractable(ifNoInteractable, transform);
        }
        isHovered = true;
    }

    private void OnMouseExit() {
        if (canDragAndDropItem == true || isHovered == true && canBeInteracted == false) {
            isHovered = false;
            UI_TooltipController.Hide();
        }
    }

    private void OnBeginDragItemEvent(ItemSlotOnBeginDragEvent itemSlotEvent) {
        if (isActivated == false || debugMode == true) {
            if (itemToAccept.Equals(itemSlotEvent.ItemSlot.ItemDefinition) == true) {
                canDragAndDropItem = true;
            }
        }
    }

    private void OnEndDragItemEvent(ItemSlotOnEndDragEvent itemSlotEvent) {
        if (canDragAndDropItem == true && isHovered == true) {
            itemSlot = itemSlotEvent.ItemSlot;
            isActivated = true;
            canBeInteracted = true;
            UI_TooltipController.Hide();
            actor = itemSlotEvent.ItemSlot.InventorySide == InventorySide.Left
                ? PlayerController.ActorLeft
                : PlayerController.ActorRight;


            if (TryGetComponent(out Interactable interactable) == true) {
                actor.MoveToAndInteract(interactable);
            }

            if (actor != null) {
                actor.SubscribeOnTargetReached(OnTargetReached);
                actor.SubscribeOnTargetChanged(OnTargetChanged);
            }

        }
        else if (debugMode == true && canDragAndDropItem == true && isHovered == true) {
            Interact(null);
        }
    }

    private void OnTargetReached() {
        if (actor != null)
            actor.UnsubscribeTargetReached(OnTargetReached);
    }

    private void OnTargetChanged() {
        if (actor != null) {
            actor.UnsubscribeTargetChanged(OnTargetChanged);
            actor = null;
        }
        isActivated = false;
        canBeInteracted = false;
    }

    protected override void OnInteract(Actor actor) {

        if (isActivated == true && canBeInteracted == true) {
            itemSlot.Consume();
            itemSlot = null;
            isActivated = false;
            SendSignal(true);
        }

        if (this.actor != null) {
            this.actor.UnsubscribeTargetChanged(OnTargetChanged);
            this.actor = null;
        }
    }

    private void UseDebugEvent(UseDebugEvent debugEvent) {
        debugMode = debugEvent.Toggle;
    }

}
