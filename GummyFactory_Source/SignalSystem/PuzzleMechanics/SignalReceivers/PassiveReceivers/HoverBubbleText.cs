using UnityEngine;

public class HoverBubbleText : SignalReceiver {

    [Header("Settings")]
    [SerializeField] private bool isInteractable = true;
    [Space]
    [SerializeField] private InteractableDefinition interactableText;
    [SerializeField] private float textHeightOffset = 125f;

    [Header("Debug")]
    [SerializeField] private bool isActive;

    private void Start() {
        isActive = true;
    }

    private void OnMouseEnter() {
        if (isInteractable == true) {
            UI_TooltipController.ShowHoveredInteractable(interactableText, transform, textHeightOffset);
            isActive = true;
        }
    }

    private void OnMouseExit() {
        if (isInteractable == true) {
            UI_TooltipController.Hide();
            isActive = false;
        }
    }

    protected override bool OnReceiveSignal(bool newSignal) {
        isInteractable = newSignal;
        if (isInteractable == false) {
            UI_TooltipController.Hide();
            isActive = false;
        }
        return newSignal;
    }

}
