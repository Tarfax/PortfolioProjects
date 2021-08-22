using System;
using System.Collections;
using System.Collections.Generic;
using MC_Utility;
using UnityEngine;
using UnityEngine.InputSystem;

public class EnemySpawnerManager : MonoBehaviour {
    // Stage
    public int spawnStage;
    private int spawnStageMax;

    public GameObject[] stageHolder;

    private static bool lastStageCleared;
    private static bool bossCleared;
    private bool bossWasSpawned;
    private int bossClearedTimer;

    //********************************************************************************************************

    private void Start() {
        spawnStage = 0;
        spawnStageMax = stageHolder.Length;

        lastStageCleared = false;
        bossCleared = false;
        bossWasSpawned = false;
        bossClearedTimer = 0;

        // Start stage

        for (int i = 0; i < stageHolder.Length; i++) {
            if (i == 0)
                continue;
            stageHolder[i].SetActive(false);
        }

        //stageHolder[spawnStage].SetActive(true);

    }

    private void Update() {
        CHEAT_NextSpawnStage();

        // Boss cleared timer
        BossClearedTimer();
    }

    private void LateUpdate() {
        //bossCleared = false;
    }

    //********************************************************************************************************

    // Unregister Event
    private void OnDisable() {
        EventSystem<EnemySpawnedEvent>.UnregisterListener(OnSpawnedEvent);
    }

    // Register Event
    private void OnEnable() {
        EventSystem<EnemySpawnedEvent>.RegisterListener(OnSpawnedEvent);
    }

    private void OnSpawnedEvent(EnemySpawnedEvent spawnedEvent) {
        if (spawnedEvent.EnemyType == EnemyType.BossAlien) {
            bossWasSpawned = true;
            bossCleared = false;
        }
    }

    //********************************************************************************************************

    // Next spawn stage
    public void NextSpawnStage() {
        // Wave cleared
        if (bossWasSpawned == true) {
            bossWasSpawned = false;
            bossCleared = true;
            bossClearedTimer = 2;
        }

        if (spawnStage < stageHolder.Length) {
            // Deactivate current stage
            stageHolder[spawnStage].SetActive(false);
            spawnStage++;
            // Activate next stage
            if (spawnStage < spawnStageMax) {
                stageHolder[spawnStage].SetActive(true);
                return;
            }
        }

        lastStageCleared = true;
    }

    // Last stage cleared
    public static bool LastStageCleared() {
        return lastStageCleared;
    }

    // Boss cleared
    public static bool BossCleared() {
        return bossCleared;
    }

    private void BossClearedTimer() {
        if (bossCleared == false) {
            return;
        }

        bossClearedTimer -= 1;
        if (bossClearedTimer == 0) {
            bossCleared = false;
        }
    }

    //********************************************************************************************************

    // CHEAT next stage
    private void CHEAT_NextSpawnStage() {
        if (Keyboard.current.f2Key.wasPressedThisFrame == true) {

            NextSpawnStage();
        }
    }

}
