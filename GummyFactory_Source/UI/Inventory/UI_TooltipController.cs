using UnityEngine;

public class UI_TooltipController : MonoBehaviour {

    private static UI_TooltipController instance;
    [SerializeField] private UI_Tooltip tooltip;
    [SerializeField] private UI_PuzzleTooltip puzzleTooltip;
    private new Camera camera;

    void Awake() {
        instance = this;
        camera = Camera.main;
    }

    private void Start() {
        Hide();
    }

    public static void ShowHoveredInteractable(InteractableDefinition interactableDefinition, Transform position, float heightOffset = 125f) {
        if (interactableDefinition == null) {
            return;
        }
        instance.puzzleTooltip.SetActive(true);
        instance.puzzleTooltip.SetText(interactableDefinition);
        Vector2 cameraPosition = instance.camera.WorldToScreenPoint(position.position);
        instance.puzzleTooltip.SetPositionWorld(cameraPosition, heightOffset);
    }

    public static void ShowAboveItem(ItemDefinition itemDefinition, RectTransform transform, float heightOffset = 0) {
        instance.tooltip.SetActive(true);
        instance.tooltip.SetText(itemDefinition);
        instance.tooltip.SetPositionUI(transform.position, heightOffset);
    }

    public static void Hide() {
        instance.tooltip.SetActive(false);
        instance.puzzleTooltip.SetActive(false);
    }
}
