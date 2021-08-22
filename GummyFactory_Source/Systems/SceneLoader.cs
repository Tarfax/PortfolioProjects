using MC_Utility.EventSystem;
using UnityEngine;
using UnityEngine.SceneManagement;

public class SceneLoader : MonoBehaviour {

    [SerializeField] private int thisLevel = -1;
    [SerializeField] private int nextLevel;
    [Space]
    [SerializeField] private bool reset;

    private void OnEnable() {
        thisLevel = SceneManager.GetActiveScene().buildIndex;
        EventSystem<LevelFinishedEvent>.RegisterListener(LevelFinished);
    }

    private void OnDisable() {
        EventSystem<LevelFinishedEvent>.UnregisterListener(LevelFinished);
    }

    private void LevelFinished(LevelFinishedEvent levelFinishedEvent) {
        if (thisLevel == 3) {
            return;
        }
        SceneManager.LoadScene(nextLevel);
    }

    private void ReloadLevel() {
        SceneManager.LoadScene(thisLevel);
    }

    private void OnValidate() {
        if (reset == true) {
            thisLevel = SceneManager.GetActiveScene().buildIndex;
            nextLevel = thisLevel + 1;
            reset = false;
        }
    }

}
