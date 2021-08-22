using UnityEngine;
using MC_Utility.EventSystem;

public class UI_PlayerItemHolder : MonoBehaviour {

    [SerializeField] private GameObject itemSlotPrefab;
    [Header("UI_Canvas_PlayerItemHolder")]
    [SerializeField] private Canvas playerItemHolderCanvas;
    [SerializeField] private RectTransform baseRectTransform;
    [SerializeField] private GameObject content;

    private bool followMouse;
    
    [Header("Debug")]
    [SerializeField] private UI_ItemSlot hoveredItemSlot;
    [SerializeField] private UI_ItemSlot draggedItemSlot;

    
    private UI_ItemSlot playerItemSlot;


    private void OnEnable() {
        EventSystem<ItemSlotHoverEvent>.RegisterListener(HoveredItemSlot);
        EventSystem<ItemSlotUnhoverEvent>.RegisterListener(UnhoveredItemSlot);
    }

    private void OnDisable() {
        EventSystem<ItemSlotHoverEvent>.UnregisterListener(HoveredItemSlot);
        EventSystem<ItemSlotUnhoverEvent>.UnregisterListener(UnhoveredItemSlot);
    }

    private void Start() {
        playerItemSlot = GameObject.Instantiate(itemSlotPrefab, content.transform).GetComponent<UI_ItemSlot>();
        playerItemSlot.CanRecieveRaycast(false);
        baseRectTransform.gameObject.SetActive(false);
    }

    private void Update() {
        if (followMouse == true) {
            baseRectTransform.anchoredPosition = Input.mousePosition / playerItemHolderCanvas.scaleFactor;
        }
    }

    private void HoveredItemSlot(ItemSlotHoverEvent hoverEvent) {
        hoveredItemSlot = hoverEvent.ItemSlot;
    }

    private void UnhoveredItemSlot(ItemSlotUnhoverEvent hoverEvent) {
        hoveredItemSlot = null;
    }

    public void OnBeginDrag() {
        if (hoveredItemSlot == null) {
            return;
        }
        draggedItemSlot = hoveredItemSlot;
        SetFollowMouse(true);
        baseRectTransform.gameObject.SetActive(true);
        playerItemSlot.Initialize(hoveredItemSlot.ItemDefinition, hoveredItemSlot.InventorySide);
        playerItemSlot.OnlyUseItemSprite();

    }

    public void OnDrag() {

    }

    public void OnEndDrag() {
        if (draggedItemSlot == null) {
            return;
        }
        TryCraftItem();

        SetFollowMouse(false);
        draggedItemSlot = null;
        baseRectTransform.gameObject.SetActive(false);
    }

    private void TryCraftItem() {
        if (hoveredItemSlot!= null && draggedItemSlot.SlotNumber == hoveredItemSlot.SlotNumber) {
            hoveredItemSlot.Hover();
            return;
        }

        ItemDefinition[] itemDefinitions = new ItemDefinition[2];
        if (hoveredItemSlot != null && hoveredItemSlot.InventorySide == draggedItemSlot.InventorySide) {
            itemDefinitions[0] = hoveredItemSlot.ItemDefinition;
        }
        itemDefinitions[1] = draggedItemSlot.ItemDefinition;
        ItemDefinition itemDef = CraftingSystem.CheckIfCraftingPossible(itemDefinitions);
        if (itemDef != null) {
            Debug.Log("Created a " + itemDef.ItemType.ToString());
            hoveredItemSlot.Initialize(itemDef, hoveredItemSlot.InventorySide);
            draggedItemSlot.SetActive(false);
        }
    }

    private void SetFollowMouse(bool follow) {
        followMouse = follow;
        if (follow == true) {
            baseRectTransform.anchoredPosition = Input.mousePosition;
        }
    }

}
