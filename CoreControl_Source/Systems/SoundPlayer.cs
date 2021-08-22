using MC_Utility;
using System.Collections.Generic;
using UnityEngine;

class SoundPlayer : MonoBehaviour
{
	// Singleton instance.
	public static SoundPlayer Instance = null;

	private Transform cameraTransform;

	[SerializeField] private List<AudioSource> sounds; //prefabs

	private Dictionary<AudioSource, AudioSource> liveSounds = new Dictionary<AudioSource, AudioSource>(); //prefab -> live

	private void Awake()
	{
		if (Instance == null)
		{
			Instance = this;
		}
		else if (Instance != this)
		{
			Destroy(gameObject);
		}
		DontDestroyOnLoad(gameObject);

		cameraTransform = Camera.main.transform;
				
		for (int i = 0; i < sounds.Count; i++)
		{
			liveSounds[sounds[i]] = Instantiate(sounds[i],transform);
		}

	}

    private void OnEnable()
    {
		EventSystem<FireEvent>.RegisterListener(PlayFireEventSound);
    }

    private void OnDisable()
    {
		EventSystem<FireEvent>.UnregisterListener(PlayFireEventSound);
	}

	private void PlayFireEventSound(FireEvent fireEvent)
    {
		PlayRandomSound(fireEvent.fireSounds);
    }


    public void PlaySound(AudioSource prefab)
	{
		liveSounds[prefab].PlayOneShot(prefab.clip);
	}

	public void PlayRandomSound(AudioSource[] prefabs)
	{
        if (prefabs.Length > 0)
        {
			System.Random rand = new System.Random();
			int index = rand.Next(0, prefabs.Length);
			PlaySound(prefabs[index]);
        }
	}

	public void PlaySound(AudioClip clip, float volume = 1f)
	{
		AudioSource.PlayClipAtPoint(clip, cameraTransform.position, volume);
	}

	public void PlayRandomSound(AudioClip[] clips, float volume = 1f)
	{
		System.Random rand = new System.Random();
		int index = rand.Next(0, clips.Length);
		AudioSource.PlayClipAtPoint(clips[index], cameraTransform.position, volume);
	}


}
