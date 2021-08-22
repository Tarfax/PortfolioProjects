using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ParticleEffectController : MonoBehaviour {
    ParticleSystem particleSys;
    [Range(5f, 50f)] [SerializeField] float emissionRate = 25f;
    private float lastEmissionRate;
    // Start is called before the first frame update
    void Start() {
        particleSys = gameObject.GetComponent<ParticleSystem>();
        if (particleSys == null) {
            Debug.Log("WTF");
        }
        lastEmissionRate = emissionRate;
    }

    // Update is called once per frame
    void Update() {
        if (lastEmissionRate != emissionRate) {
            Debug.Log("Change");
            ParticleSystem.EmissionModule emission = particleSys.emission;
            emission.rateOverTime = emissionRate;
            lastEmissionRate = emissionRate;
        }
    }
}
