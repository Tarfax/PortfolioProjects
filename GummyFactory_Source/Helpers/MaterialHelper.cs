using UnityEngine;

public class MaterialHelper : MonoBehaviour {

    private static MaterialHelper instance;
    [SerializeField] private Material outlineMaterial;
    [SerializeField] private Material signalLampMaterial;
    [SerializeField] private GameObject particleEffect;

    private void OnEnable() {
        if (instance == null) {
            instance = this;
        }
    }

    public static Material GetOutlineMaterial() {
        return instance.outlineMaterial;
    }

    public static Material GetSignalLampMaterial() {
        return instance.signalLampMaterial;
    }

    public static GameObject GetParticleOutline() {
        return instance.particleEffect;
    }

}
