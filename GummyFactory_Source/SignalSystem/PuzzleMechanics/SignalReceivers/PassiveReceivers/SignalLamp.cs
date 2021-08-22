using UnityEngine;

public class SignalLamp : SignalReceiver {

    private readonly string emissionColor = "_EmissionColor";
    [Header("Settings")]
    [SerializeField] private bool startSignal;
    [ColorUsage(true, true)] [SerializeField] private Color greenColor = Color.green * 5f;
    [ColorUsage(true, true)] [SerializeField] private Color redColor = Color.red * 5f;
    [Header("References")]
    [SerializeField] Renderer meshRenderer;
    private Material material;
    private bool currentSignal;

    private void Start() {
        signal = currentSignal = startSignal;
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
        currentSignal = newSignal;
        if (currentSignal == true) {
            meshRenderer.material.SetColor(emissionColor, greenColor);
        }
        else {
            meshRenderer.material.SetColor(emissionColor, redColor);
        }
        return currentSignal;
    }

}
