using UnityEngine;

public class DebugSendSignal : SignalSender {

    [SerializeField] private KeyCode keyToPress;

    void Update() {
        if (Input.GetKeyDown(keyToPress) == true) {
            SendSignal(true);
        }
    }
}
