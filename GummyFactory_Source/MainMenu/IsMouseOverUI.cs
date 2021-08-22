using System;
using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;
using UnityEngine.Events;
using UnityEngine.EventSystems;
using UnityEngine;
using System.Collections;
using UnityEngine.UI;
using UnityEngine.EventSystems;// Required when using Event data.

public class IsMouseOverUI : MonoBehaviour, IPointerEnterHandler, IPointerExitHandler
{
    public UnityEvent onCompleteCallBack;
    public Vector3 Startscale = Vector3.zero;
    public Vector3 Normalscale = Vector3.one;
    public Vector3 HoverScale = new Vector3(1.25f, 1.25f, 1.25f);
    public bool OverUI = false;
    private bool HasEnter = false;

    void Update()
    {
        if (OverUI == true && HasEnter == false)
        {
            HasEnter = true;
            LeanTween.scale(gameObject, HoverScale, 0.1f).setDelay(0).setOnComplete(onComplete);    
        }
        else if (OverUI == false && HasEnter == true)
        {
            HasEnter = false;
            LeanTween.scale(gameObject, Normalscale, 0.1f).setDelay(0).setOnComplete(onComplete);   
        }
    }

    private void onComplete()
    {
        if (onCompleteCallBack != null)
        {
            onCompleteCallBack.Invoke();
        }
    }
    
    public void OnPointerEnter(PointerEventData eventData)
    {
        OverUI = true;
    }

    public void OnPointerExit(PointerEventData eventData)
    {
        OverUI = false;
    }
}
