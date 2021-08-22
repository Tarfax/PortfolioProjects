using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Events;
using UnityEngine.EventSystems;

public class ScaleTween : MonoBehaviour
{
    public UnityEvent onCompleteCallBack;
    public Vector3 scale = Vector3.zero;
    public Vector3 hoverScale = new Vector3(1.25f, 1.25f, 1.25f);
    public float Yoffset = 0;
    //public Transform rootObject;
    public bool OverUI = false;
    
    private bool HasEnter = false;
    private bool Enter = false;
    private bool Exit = false;
    

    
    public void Start()
    {
        transform.localScale = new Vector3(0, 0, 0);
        LeanTween.scale(gameObject, scale, 0.3f).setDelay(0.5f).setOnComplete(onComplete);
    }

    private void Update()
    {


        //transform.position = new Vector3(rootObject.position.x,rootObject.position.y + Yoffset,0);
    }

    private void onComplete()
    {
        if (onCompleteCallBack != null)
        {
            onCompleteCallBack.Invoke();
        }
    }

    public void OnClose()
    {
        LeanTween.scale(gameObject, new Vector3(0f, 0f, 0f), 0.5f).setOnComplete(DestroyMe);
    }


    


    void DestroyMe()
    {
        Destroy(gameObject);
    }
}
