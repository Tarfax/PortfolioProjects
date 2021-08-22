using System.Collections.Generic;
using _Game.Scripts.Systems.Sound;
using MC_Utility.EventSystem;
using UnityEngine;

public class TortureChamber : MonoBehaviour {

    private readonly string _explode = "_Explode";
    private readonly string _splatter = "_Splatter";

    [Header("References")]
    [SerializeField] private MeshRenderer meshRenderer;
    [SerializeField] private GameObject prefabToSpawn;
    [SerializeField] private Transform positionToSpawn;
    [SerializeField] private List<GameObject> sewers;
    private Material material;
    private Wiggle wiggle;
    private Animator wiggleAnimator;

    [Header("Shader Settings")]
    [SerializeField] private float explodeMin = 0.12f;
    [SerializeField] private float explodeMax = 0.1f;
    [Space]
    [SerializeField] private float splatterMin = 15f;
    [SerializeField] private float splatterMax = 0f;

    [Header("Wiggle Settings")]
    [SerializeField] private float wigglePosRangeMin = 0f;
    [SerializeField] private float wigglePosRangeMax = 0.2f;
    [SerializeField] private float wiggleRotRangeMin = 0f;
    [SerializeField] private float wiggleRotRangeMax = 0.2f;
    [Space]
    [SerializeField] private float startScalePercent = 0.01f;
    [SerializeField] private float scaleAmount = 2f;
    [SerializeField] private float splatTimeBeforeDestroy = 0.05f;

    [Header("Timer")]
    [SerializeField] private float wiggleTime = 12f;
    [SerializeField] private float wiggleTimer;
    [SerializeField] private SoundDefinition wiggleSound;
    [Space]

    [SerializeField] private float splatterTime = 0.12f;
    [SerializeField] private float splatterTimer;
    [SerializeField] private SoundDefinition splatterSound;
    private bool playSplatterSound = false;
    [Space]

    [SerializeField] private float spawnTime = 6.0f;
    [SerializeField] private float spawnTimer;
    [SerializeField] private SoundDefinition spawnSound;

    [Space]

    [SerializeField] private float cleanUpTime = 10f;
    [SerializeField] private float cleanUpTimer;

    private float wiggleDelta;
    private float spawnDelta;
    private float cleanUpDelta;

    private bool isSpawn = false;
    private bool isKill = false;
    private bool isWiggle = false;

    private void OnEnable() {
        EventSystem<LevelFinishedEvent>.RegisterListener(LevelFinished);
    }

    private void OnDisable() {
        EventSystem<LevelFinishedEvent>.UnregisterListener(LevelFinished);
    }

    private void Start() {
        isSpawn = true;

        material = meshRenderer.material;
        material.SetFloat(_explode, explodeMin);
        material.SetFloat(_splatter, splatterMin);
    }
    private void Update() {
        if (isWiggle == true)
            Wiggle();

        if (isSpawn == true)
            Spawn();

        if (isKill == true) {
            Kill();
        }
    }

    private void Wiggle() {
        wiggleTimer -= Time.deltaTime;
        if (wiggleTimer >= 0f) {
            wiggleDelta = wiggleTimer / wiggleTime;

            wiggle.posRange = Mathf.Lerp(wigglePosRangeMax, wigglePosRangeMin, wiggleDelta);
            wiggle.rotRange = Mathf.Lerp(wiggleRotRangeMax, wiggleRotRangeMin, wiggleDelta);

            if (wiggleAnimator != null) {
                wiggleAnimator.speed = Mathf.Lerp(3f, 0.5f, wiggleDelta);
            }

            if (wiggleDelta <= startScalePercent) {
                if (wiggleAnimator != null) {
                    wiggle.transform.localScale += Vector3.one * scaleAmount * Time.deltaTime;
                    wiggleAnimator.speed = 0.2f;
                }

            }

            cleanUpTimer -= Time.deltaTime;
            cleanUpDelta = cleanUpTimer / cleanUpTime;
            cleanUpDelta = Mathf.Clamp(cleanUpDelta, 0.1f, 1f);
            material.SetFloat(_explode, Mathf.Lerp(explodeMin, explodeMax, cleanUpDelta));
            material.SetFloat(_splatter, Mathf.Lerp(splatterMin, splatterMax, cleanUpDelta));
        }
        else if (wiggleTimer <= 0f) {
            isWiggle = false;

        }

        if (wiggleTimer <= splatTimeBeforeDestroy) {
            splatterTimer = splatterTime;
            isKill = true;
            if (playSplatterSound == false) {
                if (splatterSound != null && splatterSound.AudioClips != null) {
                    SoundManager.PlaySFX(splatterSound.AudioClips, transform.position, splatterSound.VolumeMultiplier, splatterSound.PitchMultiplier);
                }
                playSplatterSound = true;
            }
        }
    }

    private void Kill() {
        if (wiggle != null) {
            wiggle.Destroy(splatTimeBeforeDestroy);
        }

        splatterTimer -= Time.deltaTime;

        spawnDelta = splatterTimer / splatterTime;
        material.SetFloat(_explode, Mathf.Lerp(explodeMax, explodeMin, spawnDelta));
        material.SetFloat(_splatter, Mathf.Lerp(splatterMax, splatterMin, spawnDelta));

        if (splatterTimer <= 0f) {
            spawnTimer = spawnTime;
            cleanUpTimer = cleanUpTime;
            cleanUpDelta = 1f;
            isSpawn = true;
            isKill = false;
            foreach (var item in sewers) {
                item.SetActive(true);
            }
        }
    }

    private void Spawn() {
        spawnTimer -= Time.deltaTime;
        if (spawnTimer <= 0f) {
            wiggle = Instantiate(prefabToSpawn, positionToSpawn, false).GetComponentInChildren<Wiggle>();
            wiggleAnimator = wiggle.GetComponent<Animator>();

            if (spawnSound != null && spawnSound.AudioClips != null) {
                SoundManager.PlaySFX(spawnSound.AudioClips, transform.position, spawnSound.VolumeMultiplier, spawnSound.PitchMultiplier);
            }

            wiggleTimer = wiggleTime;
            cleanUpTimer = cleanUpTime;
            cleanUpDelta = 1f;
            isWiggle = true;
            isSpawn = false;
            playSplatterSound = false;
            if (wiggleSound != null && wiggleSound.AudioClips != null) {
                SoundManager.PlaySFX(wiggleSound.AudioClips, transform.position, wiggleSound.VolumeMultiplier, wiggleSound.PitchMultiplier);
            }

            foreach (var item in sewers) {
                item.SetActive(false);
            }
        }
    }

    public void TurnOff() {
        isWiggle = false;
        isSpawn = false;
        isKill = false;
        wiggle.TurnOff();
    }

    private void LevelFinished(LevelFinishedEvent levelFinishedEvent) {
        TurnOff();
    }

}
