using System;
using UnityEngine.EventSystems;
using UnityEngine;
using UnityEngine.UI;
using TMPro;
using MC_Utility.EventSystem;

public abstract class UI_ItemSlot : MonoBehaviour {
    public int Count { get; set; }
    private int temporaryCount;
    public ItemDefinition ItemDefinition { get; protected set; }
    private ItemDefinition temporaryItem;

    [Header("References")]
    [SerializeField] private Image itemSprite;
    [SerializeField] private Sprite itemSpriteDark;
    [SerializeField] private Image itemSpriteBackground;
    //[SerializeField] private Image itemNameBackground;
    //[SerializeField] private TextMeshProUGUI text;
    //[SerializeField] private UI_Tooltip tooltip;

    [Header("Debug")]
    [SerializeField] private InventorySide inventorySide;
    public InventorySide InventorySide { get => inventorySide; protected set { inventorySide = value; } }
    [SerializeField] private int slotNumber;
    public int SlotNumber { get => slotNumber; private set { slotNumber = value; } }

    public bool IsEmpty { get { return Count == 0; } }

    private bool isDragged;
    [SerializeField] private bool isActiveInSlot;
    public bool IsActiveInSlot { get => isActiveInSlot; private set { isActiveInSlot = value; } }

    private void Start() {
        UI_MouseController mouseController = gameObject.AddComponent<UI_MouseController>();
        mouseController.RegisterOnPointerEnter(OnPointerEnter);
        mouseController.RegisterOnPointerExit(OnPointerExit);
        mouseController.RegisterOnPointerDown(OnPointerDown);
        mouseController.RegisterOnBeginDrag(OnBeginDrag);
        mouseController.RegisterOnEndDrag(OnEndDrag);
        //isInCraftingInventory = false;
        IsActiveInSlot = true;
    }

    protected virtual void OnBeginDrag(PointerEventData eventData) {
        if (IsActiveInSlot == true) {
            isDragged = true;
            itemSpriteBackground.color = new Color(0.35f, 0.35f, 0.35f, 1f);
            itemSpriteBackground.sprite = itemSprite.sprite;
            itemSprite.color = new Color(0.0f, 0.0f, 0.0f, 0.35f);
            EventSystem<ItemSlotOnBeginDragEvent>.FireEvent(ItemSlotOnBeginDragEvent.GetEvent(this));
        }
    }

    protected virtual void OnDrag(PointerEventData eventData) { }

    protected virtual void OnEndDrag(PointerEventData eventData) {
        if (IsActiveInSlot == true) {
            itemSpriteBackground.color = new Color(0.0f, 0.0f, 0.0f, 0.0f);
            itemSprite.color = new Color(1.0f, 1.0f, 1.0f, 1.0f);

        }
        EventSystem<ItemSlotOnEndDragEvent>.FireEvent(ItemSlotOnEndDragEvent.GetEvent(this));
        isDragged = false;
    }

    protected void OnPointerEnter(PointerEventData eventData) {
        if (IsActiveInSlot == true) {
            if (isDragged == false) {
                //itemSpriteBackground.color = new Color(0.2f, 0.2f, 0.2f, 0.65f);
            }
            EventSystem<ItemSlotHoverEvent>.FireEvent(ItemSlotHoverEvent.GetEvent(this));
            UI_TooltipController.ShowAboveItem(ItemDefinition, GetComponent<RectTransform>(), 100f);
            //tooltip.SetText(ItemDefinition.ItemDescription);
        }
    }

    protected virtual void OnPointerExit(PointerEventData eventData) {
        if (IsActiveInSlot == true) {
            if (isDragged == false) {
                itemSpriteBackground.color = new Color(0, 0, 0, 0.0f);
            }
            EventSystem<ItemSlotUnhoverEvent>.FireEvent(ItemSlotUnhoverEvent.GetEvent());
            UI_TooltipController.Hide();
        }
    }

    protected virtual void OnPointerUp(PointerEventData eventData) { }

    protected void OnPointerDown(PointerEventData eventData) {

    }


    public void Initialize(ItemDefinition itemDefinition, InventorySide side, bool isCrafted = false) {
        ItemDefinition = itemDefinition;
        itemSprite.sprite = ItemDefinition.Sprite;
        //text.text = ItemDefinition.ItemType.ToString();
        InventorySide = side;
        SetActive(true);
        gameObject.name = "ItemSlot " + itemDefinition.ItemType.ToString();
    }

    public void AssignItemSlot(int slotNumber) {
        SlotNumber = slotNumber;
    }

    public void CanRecieveRaycast(bool canRecieve) {
        itemSprite.raycastTarget = false;
        //itemNameBackground.raycastTarget = false;
    }

    public void OnlyUseItemSprite() {
        //itemNameBackground.gameObject.SetActive(false);
        itemSpriteBackground.gameObject.SetActive(false);
    }

    public void SetActive(bool beActive) {
        if (beActive == true) {
            //itemNameBackground.gameObject.SetActive(true);
            itemSpriteBackground.gameObject.SetActive(true);
            itemSprite.gameObject.SetActive(true);
            IsActiveInSlot = true;
        }
        else {
            //itemNameBackground.gameObject.SetActive(false);
            itemSpriteBackground.gameObject.SetActive(false);
            itemSprite.gameObject.SetActive(false);
            IsActiveInSlot = false;
        }
    }

    public void Consume() {
        SetActive(false);
    }

    public void Hover() {
        OnPointerEnter(null);
    }

}

public class ItemSlotHoverEvent : IEvent {
    public UI_ItemSlot ItemSlot;

    public static ItemSlotHoverEvent GetEvent(UI_ItemSlot itemSlot) {
        return new ItemSlotHoverEvent() { ItemSlot = itemSlot };
    }
}

public class ItemSlotUnhoverEvent : IEvent {
    public static ItemSlotUnhoverEvent GetEvent() {
        return new ItemSlotUnhoverEvent();
    }
}

public class ItemSlotOnBeginDragEvent : IEvent {
    public UI_ItemSlot ItemSlot;

    public static ItemSlotOnBeginDragEvent GetEvent(UI_ItemSlot itemSlot) {
        return new ItemSlotOnBeginDragEvent() { ItemSlot = itemSlot };
    }

}

public class ItemSlotOnEndDragEvent : IEvent {
    public UI_ItemSlot ItemSlot;

    public static ItemSlotOnEndDragEvent GetEvent(UI_ItemSlot itemSlot) {
        return new ItemSlotOnEndDragEvent() { ItemSlot = itemSlot };
    }
}