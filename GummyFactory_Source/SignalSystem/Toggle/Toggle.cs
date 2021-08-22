using System.Collections.Generic;
using UnityEngine;

public class Toggle : MonoBehaviour {

    private static List<Toggle> toggles;

    [SerializeField] private bool shouldBeState;

    [SerializeField] private bool toggle;
    [SerializeField] private List<Toggle> connectedToggles;

    void Start() {
        if (toggles == null) {
            toggles = new List<Toggle>();
        }
        toggles.Add(this);
    }

    private void OnValidate() {
        if (connectedToggles.Contains(this) == false) {
            connectedToggles.Add(this);
        }
    }

    public void AddToggles(List<Toggle> toggles) {
        if (toggles.Contains(this) == false) {
            connectedToggles = new List<Toggle>();
            connectedToggles.Add(this);
            return;
        }

        List<Toggle> tempList = new List<Toggle>();

        foreach (Toggle toggle in toggles) {
            if (toggle != null) {
                tempList.Add(toggle);
            }
        }

        connectedToggles = new List<Toggle>(tempList);
    }

    public bool TestLogic() {
        if (toggle != shouldBeState) {
            return false;
        }

        return true;
    }

}
