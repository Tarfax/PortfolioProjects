using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SimpleReceiver : SignalReceiver {
    protected override bool OnReceiveSignal(bool newSignal) {
        Debug.Log("newSignal " + newSignal);
        return newSignal;
    }
}
