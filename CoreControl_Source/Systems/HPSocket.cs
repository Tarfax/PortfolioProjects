using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class HPSocket : MonoBehaviour
{
    [SerializeField] private GameObject[] shine;

    [SerializeField] private ParticleSystem sparkle;

    public void Half()
    {
        shine[0].SetActive(false);
        shine[1].SetActive(true);
        sparkle.Play();
    }

    public void Off()
    {
        shine[1].SetActive(false);
        sparkle.Play();
    }

}
