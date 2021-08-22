using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ToggleConnector : MonoBehaviour {

    [SerializeField] private List<Toggle> connectedToggles;
    private List<Toggle> connectedTogglesRedo;

    // Start is called before the first frame update
    void Start() {

    }

    // Update is called once per frame
    void Update() {

    }

    private void OnValidate() {
        List<Toggle> tempList = new List<Toggle>();
        foreach (Toggle toggle in connectedToggles) {
            if (toggle != null) {
                tempList.Add(toggle);
            }
        }

        foreach (Toggle toggle in connectedToggles) {
            if (toggle != null) {
                toggle.AddToggles(connectedToggles);
            }
        }

        Toggle removedToggle;
        foreach (Toggle toggle in connectedTogglesRedo) {
            removedToggle = toggle;
            foreach (Toggle toggle1 in connectedToggles) {
                if (toggle == toggle1) {
                    removedToggle = null;
                }
            }
            if (removedToggle != null) {
                removedToggle.AddToggles(connectedToggles);
            }
        }

        connectedTogglesRedo = new List<Toggle>(tempList);
        connectedToggles = new List<Toggle>(tempList);
    }


}
