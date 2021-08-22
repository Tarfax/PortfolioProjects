using System;
using UnityEngine;
using UnityEngine.EventSystems;

public class UI_MouseController : MonoBehaviour, IPointerEnterHandler, IPointerExitHandler, IPointerDownHandler, IPointerUpHandler, IBeginDragHandler, IEndDragHandler, IDragHandler {

    private Action<PointerEventData> onPointerEnter;
    private Action<PointerEventData> onPointerExit;
    private Action<PointerEventData> onBeginDrag;
    private Action<PointerEventData> onDrag;
    private Action<PointerEventData> onEndDrag;
    private Action<PointerEventData> onPointerDown;
    private Action<PointerEventData> onPointerUp;

    public void OnBeginDrag(PointerEventData eventData) {
        if (onBeginDrag != null)
            onBeginDrag(eventData);
    }

    public void OnDrag(PointerEventData eventData) {
        if (onDrag != null)
            onDrag(eventData);
    }

    public void OnEndDrag(PointerEventData eventData) {
        if (onEndDrag != null)
            onEndDrag(eventData);
    }

    public void OnPointerEnter(PointerEventData eventData) {
        if (onPointerEnter != null)
            onPointerEnter(eventData);
    }

    public void OnPointerExit(PointerEventData eventData) {
        if (onPointerExit != null)
            onPointerExit(eventData);
    }

    public void OnPointerDown(PointerEventData eventData) {
        if (onPointerDown != null) {
            onPointerDown(eventData);
        }
    }

    public void OnPointerUp(PointerEventData eventData) {
        if (onPointerUp != null) {
            onPointerUp(eventData);
        }
    }

    public void RegisterOnPointerEnter(Action<PointerEventData> callbackFunc) {
        onPointerEnter += callbackFunc;
    }

    public void RegisterOnPointerExit(Action<PointerEventData> callbackFunc) {
        onPointerExit += callbackFunc;
    }

    public void RegisterOnBeginDrag(Action<PointerEventData> callbackFunc) {
        onBeginDrag += callbackFunc;
    }

    public void RegisterOnDrag(Action<PointerEventData> callbackFunc) {
        onDrag += callbackFunc;
    }

    public void RegisterOnEndDrag(Action<PointerEventData> callbackFunc) {
        onEndDrag += callbackFunc;
    }

    public void RegisterOnPointerDown(Action<PointerEventData> callbackFunc) {
        onPointerDown += callbackFunc;
    }

    public void RegisterOnPointerUp(Action<PointerEventData> callbackFunc) {
        onPointerUp += callbackFunc;
    }

    public void UnregisterOnPointerEnter(Action<PointerEventData> callbackFunc) {
        onPointerEnter -= callbackFunc;
    }

    public void UnregisterOnPointerExit(Action<PointerEventData> callbackFunc) {
        onPointerExit -= callbackFunc;
    }

    public void UnregisterOnBeginDrag(Action<PointerEventData> callbackFunc) {
        onBeginDrag -= callbackFunc;
    }

    public void UnregisterOnDrag(Action<PointerEventData> callbackFunc) {
        onDrag -= callbackFunc;
    }

    public void UnregisterOnEndDrag(Action<PointerEventData> callbackFunc) {
        onEndDrag -= callbackFunc;
    }

    public void UnregisterOnPointerDown(Action<PointerEventData> callbackFunc) {
        onPointerDown -= callbackFunc;
    }

    public void UnregisterOnPointerUp(Action<PointerEventData> callbackFunc) {
        onPointerUp -= callbackFunc;
    }

}
