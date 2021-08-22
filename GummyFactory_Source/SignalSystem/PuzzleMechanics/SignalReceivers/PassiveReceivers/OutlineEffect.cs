using UnityEngine;

public class OutlineEffect : SignalReceiver {

    [Header("Settings")]
    [SerializeField] private bool isInteractable = true;

    [Header("References")]
    [SerializeField] private Renderer meshRenderer;
    private Material outlineMaterial;
    private Material defaultMaterial;

    [Header("Debug")]
    [SerializeField] private bool isActive;

    void Start() {
        defaultMaterial = meshRenderer.material;
        outlineMaterial = new Material(MaterialHelper.GetOutlineMaterial());
    }

    private void OnMouseEnter() {
        if (isInteractable == true) {
            meshRenderer.material = outlineMaterial;
            isActive = true;
        }
    }

    private void OnMouseExit() {
        if (isInteractable == true) {
            meshRenderer.material = defaultMaterial;
            isActive = false;
        }
    }

    protected override bool OnReceiveSignal(bool newSignal) {
        isInteractable = newSignal;
        if (isInteractable == false) {
            meshRenderer.material = defaultMaterial;
            isActive = false;
        }
        return newSignal;
    }

}