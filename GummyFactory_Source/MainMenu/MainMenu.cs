using _Game.Scripts.Systems.Sound;
using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

public class MainMenu : MonoBehaviour
{
    public string level1;
    public string level2;
    public string level3;

    public GameObject VolumeSliderGameObject;
    private Slider VolumeSlider;

    private float volume = 1;

    public bool inSettings = false;

    private void Awake()
    {
        VolumeSlider = VolumeSliderGameObject.GetComponent<Slider>();

        if (AudioListener.volume == 1)
        {
            AudioListener.volume = 0.25f;
        } else {
            volume = PauseMenu1.savedVolume;
            AudioListener.volume = volume;
        }
        VolumeSlider.value = AudioListener.volume;
        VolumeSliderGameObject.SetActive(false);
    }
    private void Update()
    {
        settings();
    }

    public void Level1() {
        SceneManager.LoadScene(level1);
    }

    public void Level2() {
        SceneManager.LoadScene(level2);
    }

    public void Level3() {
        SceneManager.LoadScene(level3);
    }

    public void QuitGame() {
        Application.Quit();
    }
    public void InSettings()
    {
        inSettings = !inSettings;
    }
    public void settings()
    {
        if (inSettings == false)
        {
            return;
        }

        VolumeSliderGameObject.SetActive(true);

        if (Input.GetKeyDown(KeyCode.Escape))
        {
            VolumeSliderGameObject.SetActive(false);
            inSettings = false;
        }
    }
    public void ChangeVolume()
    {
        volume = VolumeSlider.value;
        AudioListener.volume = volume;
        PauseMenu1.savedVolume = volume;
    }
}
