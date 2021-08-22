using MC_Utility.EventSystem;
using UnityEngine;

public class DebugMode : MonoBehaviour {

    [SerializeField] private GameObject debugger;

    private void OnEnable() {
        EventSystem<UseDebugEvent>.RegisterListener(ShowDebugText);
        debugger.SetActive(false);
    }

    private void OnDisable() {
        EventSystem<UseDebugEvent>.UnregisterListener(ShowDebugText);
    }

    private void ShowDebugText(UseDebugEvent debugEvent) {
        debugger.SetActive(debugEvent.Toggle);
    }
}
