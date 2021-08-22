using UnityEngine;
using UnityEngine.Events;

public class FunctionCaller : SignalReceiver {

    private bool callFunction;
    [SerializeField] private UnityEvent events;

    protected override bool OnReceiveSignal(bool newSignal) {
        if (callFunction == false && newSignal == true) {
            callFunction = true;
            events.Invoke();
            return true;
        }
        else if (callFunction == true) {
            return true;
        }
        return false;
    }
}