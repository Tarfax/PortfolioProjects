using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Experimental.GlobalIllumination;
using UnityEngine.SceneManagement;
using UnityEngine.UIElements;
using UnityEngine.UI;
using Slider = UnityEngine.UI.Slider;
using _Game.Scripts.Systems.Sound;

public class PauseMenu1 : MonoBehaviour
{
    public bool isPaused;
    public string mainMenuScene;

    public GameObject Panal;
    public GameObject PausedImage;

    public GameObject SliderGameObject;

    [SerializeField,HideInInspector] private Slider VolumeControll = null;
    private bool CanGoBack = false;
    [SerializeField] public bool Mute = false;
    [SerializeField] private float volume;
    public static float savedVolume;

    //public GameObject Paused;
    //public GameObject ResumeGameObject;
    //public GameObject MainMenyObject;
    //public GameObject SettingsMeny;

    private void Start() {
        volume = savedVolume;
        AudioListener.volume = volume;
        VolumeControll = SliderGameObject.GetComponent<Slider>();
        VolumeControll.value = AudioListener.volume;
    }


    void OnValidate()
    {
        if (VolumeControll == null)
        {
            VolumeControll = SliderGameObject.GetComponent<Slider>();    
            VolumeControll.value = AudioListener.volume;
        }
        SetActiveToFalse();
    }

    void Update()
    {
        if(Input.GetKeyDown(KeyCode.Escape))
        {
            if(isPaused == true && CanGoBack == true )
            {
                ResumeGame();
            }
            else
            {
                PauseGame();
            }
        }
    }

    public void SetActiveToFalse()
    {
        Debug.Log("SetActiveToFalse");
        Panal.SetActive(false);
        PausedImage.SetActive(false);
        SliderGameObject.SetActive(false);
        //Panal.SetActive(false);
        //MainMenyObject.SetActive(false);
        //ResumeGameObject.SetActive(false);
        //SettingsMeny.SetActive(false);
        //Paused.SetActive(false);
    }

    public void ResumeGame()
    {
        Debug.Log("ResumeGame");
        CanGoBack = true;
        isPaused = false;
        SetActiveToFalse();
        Time.timeScale = 1f;
    }

    public void PauseGame()
    {
        Debug.Log("PauseGame");
        CanGoBack = true;
        isPaused = true;
        PausedImage.SetActive(true);
        Panal.SetActive(true);
        SliderGameObject.SetActive(true);
        Time.timeScale = 0f;
        //Paused.SetActive(true);
        //MainMenyObject.SetActive(true);
        //ResumeGameObject.SetActive(true);
        //SettingsMeny.SetActive(true);
    }

    public void ReturnToMain()
    {
        Debug.Log("ReturnToMain");
        Time.timeScale = 1f;
        SceneManager.LoadScene(mainMenuScene);
    }

    public void Settings()
    {
        Debug.Log("Settings");
        CanGoBack = false;
        //MainMenyObject.SetActive(false);
        //ResumeGameObject.SetActive(false);
        //SettingsMeny.SetActive(false);
        

        if (Input.GetKeyDown(KeyCode.Escape))
        {
            PauseGame();
            //return;
        }
    }

    public void MuteUnmute()
    {
        
        Mute = !Mute;

        if (Mute == false)
        {
            ChangeVolume();
            AudioListener.volume = volume;
            //Mute = true;
        }
        else if (Mute == true)
        {
            volume = AudioListener.volume;
            AudioListener.volume = 0;
            //Mute = false;
        }
    }

    public void ChangeVolume()
    {
        volume = VolumeControll.value;
        
        if (Mute == false)
        {
            AudioListener.volume = volume;
        }
        else
        {
            //SavedVolume = VolumeControll.value;
        }

        savedVolume = volume;
    }

    public void Reload()
    {
        Time.timeScale = 1f;
        SceneManager.LoadScene(SceneManager.GetActiveScene().buildIndex);
    }

}
