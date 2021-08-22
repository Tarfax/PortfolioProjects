using MC_Utility.EventSystem;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;

public class UI_InputController : MonoBehaviour {

    private static UI_InputController instance;

    private EventSystem eventSystem;
    private new Camera camera;

    private bool useDebug;

    [SerializeField] private UI_PlayerItemHolder playerItemHolder;

    private Vector2 mousePositionLastFrame;
    private Vector2 mousePositionThisFrame;

    private Vector2 mouseStartDragPosition;
    public static bool IsDragging { get; private set; }
    private float dragDistance;
    private float dragDistanceThreshold = 2f;

    private bool leftMouseButtonDown;
    private bool leftMouseButtonUp;
    private bool rightMouseButtonUp;

    void Start() {
        if (instance == null) {
            instance = this;
        }
        eventSystem = EventSystem.current;
        camera = Camera.main;
    }

    // Update is called once per frame
    void Update() {
        mousePositionLastFrame = mousePositionThisFrame;
        mousePositionThisFrame = Input.mousePosition;

        if (Input.GetKeyUp(KeyCode.Mouse0) == true) {
            leftMouseButtonDown = false;
            leftMouseButtonUp = true;
        }

        if (IsMouseOverUI() == true) {

            if (leftMouseButtonDown == false && Input.GetKeyDown(KeyCode.Mouse0) == true) {
                leftMouseButtonDown = true;
                mouseStartDragPosition = Input.mousePosition;
            }

            if (leftMouseButtonDown == true) {
                dragDistance = Vector2.Distance(mouseStartDragPosition, mousePositionThisFrame);
                if (IsDragging == false && dragDistance > dragDistanceThreshold) {
                    IsDragging = true;
                    playerItemHolder.OnBeginDrag();
                }
                else if (IsDragging == true) {
                    playerItemHolder.OnDrag();
                }
            }


        }

        if (leftMouseButtonUp == true) {
            if (IsDragging == true) {
                leftMouseButtonDown = false;
                leftMouseButtonUp = false;
                IsDragging = false;
                playerItemHolder.OnEndDrag();
            }

        }

        if (Input.GetKeyDown(KeyCode.L) == true) {
            useDebug = !useDebug;
            EventSystem<UseDebugEvent>.FireEvent(UseDebugEvent.GetEvent(useDebug));
        }

        leftMouseButtonUp = false;
    }

    public static bool IsMouseOverUI() {
        return instance.eventSystem.IsPointerOverGameObject() == true;
    }

    public Vector2 GetMouseMovement() {
        Vector3 distance = mousePositionThisFrame - mousePositionLastFrame;
        return distance;
    }

}

public class UseDebugEvent : IEvent {
    public bool Toggle;
    public static UseDebugEvent GetEvent(bool toggle) {
        return new UseDebugEvent() { Toggle = toggle };
    }
}