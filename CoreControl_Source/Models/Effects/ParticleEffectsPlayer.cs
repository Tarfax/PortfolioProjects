using System.Collections.Generic;
using UnityEngine;

public class ParticleEffectsPlayer : MonoBehaviour {
    public bool playParticles;
    public bool playOnStart = false;

    [Header("All the particles")]
    [SerializeField] private List<ParticleSystem> particlesToPlay = default;
    public ParticleEffectType ParticleEffectType { get; set; }

    private void Start() {
        if (playOnStart) {
            PlayParticles();
        }
    }

    public void PlayParticles() {
        foreach (var item in particlesToPlay) {
            item.Play();
        }
    }

    private void OnValidate() {
        if (playParticles == true) {
            PlayParticles();
            playParticles = false;
        }
    }

}
