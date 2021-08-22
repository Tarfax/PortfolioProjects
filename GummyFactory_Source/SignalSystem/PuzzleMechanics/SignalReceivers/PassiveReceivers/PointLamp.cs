using UnityEngine;

public class PointLamp : SignalReceiver {

    [Header("Settings")]
    [SerializeField] private bool startSignal;
    [ColorUsage(true, true)] [SerializeField] private Color greenColor = Color.green * 5f;
    [ColorUsage(true, true)] [SerializeField] private Color redColor = Color.red * 5f;
    [Header("References")]
    [SerializeField] private new Light light;
    private bool currentSignal;

    private void Start() {
        signal = currentSignal = startSignal;
        ChangeColor();
    }

    private void ChangeColor() {
        if (currentSignal == true) {
            light.color = greenColor;
        }
        else {
            light.color = redColor;
        }
    }

    protected override bool OnReceiveSignal(bool newSignal) {
        currentSignal = newSignal;
        ChangeColor();
        return currentSignal;
    }

}