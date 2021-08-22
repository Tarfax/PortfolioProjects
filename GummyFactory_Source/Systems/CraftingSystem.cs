using System.Collections.Generic;
using UnityEngine;

public class CraftingSystem : MonoBehaviour {

    private static CraftingSystem instance;

    [SerializeField] private List<ItemRecipeDefinition> recipes;

    private void Start() {
        if (instance == null) {
            instance = this;
        }
    }

    public static ItemDefinition CheckIfCraftingPossible(ItemDefinition[] itemDefinitions) {
        bool recipeOne = false;
        bool recipeTwo = false;

        foreach (ItemRecipeDefinition recipe in instance.recipes) {
            for (int i = 0; i < itemDefinitions.Length; i++) {
                if (recipe.itemOne.Equals(itemDefinitions[i])) {
                    recipeOne = true;
                }
                if (recipe.itemTwo.Equals(itemDefinitions[i])) {
                    recipeTwo = true;
                }
            }
            if (recipeOne == true && recipeTwo == true) {

                return recipe.output;
            }
        }
        return null;
    }

}