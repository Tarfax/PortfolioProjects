using UnityEngine;

public class OutlineParticleEffect : SignalReceiver {

    [Header("Settings")]
    [SerializeField] private bool isInteractable = true;

    [Header("References")]
    private GameObject particleEffect;
    [SerializeField] private Transform particleTransform;
    //private Material outlineMaterial;
    //private Material defaultMaterial;

    [Header("Debug")]
    [SerializeField] private bool isActive;

    void Start() {
        //defaultMaterial = meshRenderer.material;
        //outlineMaterial = new Material(MaterialHelper.GetOutlineMaterial());
        particleEffect = MaterialHelper.GetParticleOutline();
        
    }

    private void OnMouseEnter() {
        if (isInteractable == true) {
            particleEffect.SetActive(true);
            if (particleEffect.transform == null) {
                Debug.Log("particleEffect.transform == null");
            }
            if (particleTransform == null) {
                Debug.Log("particleTransform == null");
            }
            if (particleEffect == null) {
                Debug.Log("particleEffect == null");
            }
            particleEffect.transform.position = particleTransform.position;
            isActive = true;
        }
    }

    private void OnMouseExit() {
        if (isInteractable == true) {
            particleEffect.SetActive(false);
            isActive = false;
        }
    }

    protected override bool OnReceiveSignal(bool newSignal) {
        isInteractable = newSignal;
        if (isInteractable == false) {
            particleEffect.SetActive(false);
            isActive = false;
        }
        return newSignal;
    }

}
