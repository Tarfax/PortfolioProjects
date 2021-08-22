using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using Unity.Mathematics;
using UnityEditor;
using UnityEngine;

public class UI_InventoryController : MonoBehaviour {

    private static UI_InventoryController instance;

    [SerializeField] private GameObject itemSlotPrefab;

    [SerializeField] private GameObject inventoryLeft;
    [SerializeField] private GameObject inventoryRight;

    [SerializeField] private List<UI_ItemSlot> inventoryItemSlots;
    static public Vector3 ItemRotation = Vector3.zero;
    public Vector3 ItemScale = Vector3.zero;

    private void OnEnable() {
        if (instance == null) {
            instance = this;
            inventoryItemSlots = new List<UI_ItemSlot>();
        }
    }

    public static void PickupItem(ItemDefinition itemDefinition, InventorySide side) {
        UI_ItemSlot itemSlot = null;
        foreach (UI_ItemSlot slot in instance.inventoryItemSlots) {
            if (slot != null && slot.IsActiveInSlot == false && side == slot.InventorySide) {
                itemSlot = slot;
                break;
            }
        }

        if (itemSlot == null)
        {
            Quaternion spawnRotation = Quaternion.Euler(ItemRotation.x,ItemRotation.y,ItemRotation.z);
            GameObject gameObject = GameObject.Instantiate(instance.itemSlotPrefab, GetInventorySide(side).transform);
            gameObject.transform.rotation = spawnRotation;
            //gameObject.transform.localScale = ItemScale.;
            itemSlot = gameObject.GetComponent<UI_ItemSlot>();
        }

        itemSlot.Initialize(itemDefinition, side);
        AddToInventory(instance.inventoryItemSlots, itemSlot);
    }

    private static GameObject GetInventorySide(InventorySide side) {
        if (side == InventorySide.Left) {
            return instance.inventoryLeft;
        }
        return instance.inventoryRight;
    }

    private static void AddToInventory(List<UI_ItemSlot> inventory, UI_ItemSlot itemSlot) {
        for (int i = 0; i < inventory.Count; i++) {
            if (inventory[i] == null) {
                inventory[i] = itemSlot;
                itemSlot.AssignItemSlot(i);
                return;
            }
        }
        itemSlot.AssignItemSlot(inventory.Count);
        inventory.Add(itemSlot);
    }

}

public enum InventorySide {
    Left,
    Right
}
