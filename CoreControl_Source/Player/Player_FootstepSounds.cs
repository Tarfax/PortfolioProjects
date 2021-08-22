using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Player_FootstepSounds : MonoBehaviour
{

    [SerializeField] private AudioClip[] footstepSounds;

    [SerializeField] private AudioSource[] footstepSoundsPrefabs;

    public void TriggerFootstepSound ()
    {        
        //SoundPlayer.Instance.PlayRandomSound(footstepSounds, 0.05f);
        SoundPlayer.Instance.PlayRandomSound(footstepSoundsPrefabs);
    }

}
