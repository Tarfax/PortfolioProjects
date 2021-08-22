using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Experimental.GlobalIllumination;
using UnityEngine.SceneManagement;
using UnityEngine.UIElements;
using UnityEngine.UI;
using Slider = UnityEngine.UI.Slider;
using MC_Utility.EventSystem;

public class PauseMenu : MonoBehaviour
{
    public bool isPaused;
    public string mainMenuScene;
    public GameObject pauseMenu;

    //public GameObject SliderGameObject;
    private Slider VolumeControll = null;
    private int BackStep = 0;

    void Start()
    {
        //VolumeControll = SliderGameObject.GetComponent<Slider>();
        //SliderGameObject.SetActive(false);
        pauseMenu.SetActive(false);
    }

    void Update()
    {
        if(Input.GetKeyDown(KeyCode.Escape))
        {
            if(isPaused == true && BackStep == 0)
            {
                ResumeGame();
            }
            else
            {
                PauseGame();
            }
        }
    }

    public void ResumeGame()
    {
        isPaused = false;
        pauseMenu.SetActive(false);
        Time.timeScale = 1f;
    }

    public void PauseGame()
    {
        isPaused = true;
        pauseMenu.SetActive(true);
        Time.timeScale = 0f;

        if(Input.GetKeyDown(KeyCode.Escape) && BackStep != 0)
        {
            BackStep--;
        }
    }

    public void ReturnToMain()
    {
        Time.timeScale = 1f;
        SceneManager.LoadScene(mainMenuScene);
    }

    public void RestartLevel() {
        EventSystem<LevelFinishedEvent>.FireEvent(LevelFinishedEvent.GetEvent());
    }

    public void Settings()
    {
        BackStep++;
        //SliderGameObject.SetActive(true);
        AudioListener.volume = VolumeControll.value;
    }

}
