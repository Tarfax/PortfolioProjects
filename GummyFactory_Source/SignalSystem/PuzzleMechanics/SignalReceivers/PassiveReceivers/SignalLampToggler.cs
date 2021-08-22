using UnityEngine;
using UnityEngine.Serialization;

public class SignalLampToggler : SignalReceiver {

    private readonly string emissionColor = "_EmissionColor";
    [Header("Settings")]
    [SerializeField/*, FormerlySerializedAs("startSignal")*/] private bool startSignal;
    [ColorUsage(true, true)] [SerializeField] private Color greenColor = Color.green * 5f;
    [ColorUsage(true, true)] [SerializeField] private Color redColor = Color.red * 5f;
    private Material material;
    [Header("References")]
    [SerializeField] MeshRenderer meshRenderer;
    [Header("Debug")]

    [SerializeField] private bool currentSignal;

    private void Start() {
        signal = startSignal;
        if (meshRenderer == null) {
            meshRenderer = GetComponent<MeshRenderer>();
        }
        material = new Material(MaterialHelper.GetSignalLampMaterial());
        meshRenderer.material = material;
        material.EnableKeyword("_EMISSION");

        ChangeMaterialColor();
    }

    private void ChangeMaterialColor() {
        if (material != null) {
            if (currentSignal == true) {
                material.SetColor(emissionColor, greenColor);
            }
            else {
                material.SetColor(emissionColor, redColor);
            }
        }
    }

    protected override bool OnReceiveSignal(bool newSignal) {
        currentSignal = !currentSignal;
        ChangeMaterialColor();
        return currentSignal;
    }

}
