using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UI;

public abstract class Dial : SignalSender {

    protected const float angleMax = 360f;

    [Header("Visibility")]
    [SerializeField] GameObject theDial;
    [SerializeField] GameObject thePanel;
    [SerializeField] private bool toggleVisibility;
    [SerializeField] private bool toggleSide;
    protected bool lastToggledSide;
    protected Side side;

    [Header("References", order = 1)]
    [SerializeField] protected GameObject dialNeedle;
    [SerializeField] protected GameObject dialIntervall;
    [SerializeField] protected Image dialIntervalImage;
    [SerializeField] protected Image dialButton;
    [SerializeField] protected Image indicator;
    [SerializeField] protected Sprite dialButtonNormal;
    [SerializeField] protected Sprite dialButtonPressed;
    [SerializeField] protected Sprite dialButtonHovered;
    [SerializeField] protected Sprite indicatorRed;
    [SerializeField] protected Sprite indicatorGreen;
    [SerializeField] protected GameObject leftSide;
    [SerializeField] protected GameObject rightSide;
    private UI_MouseController buttonController;

    [Header("Dial Needle Settings", order = 2)]
    protected float dialStartAngle = 0;
    protected float dialEndAngle = 0;
    protected float angle = 0f;

    [Header("Dial Intervall Settings", order = 3)]
    [Range(0f, 360f)] [SerializeField] protected float fillAmount = 45f;
    [Range(0f, -360f)] [SerializeField] protected float intervalAngle = 0f;

    protected bool isActivated = false;

    private void OnEnable() {
        if (buttonController == null) {
            buttonController = dialButton.gameObject.AddComponent<UI_MouseController>();
        }

        buttonController.RegisterOnPointerDown(ButtonDown);
        buttonController.RegisterOnPointerUp(ButtonUp);
        buttonController.RegisterOnPointerEnter(ButtonHovered);
        buttonController.RegisterOnPointerExit(ButtonUnhovered);

        toggleVisibility = false;
    }


    private void OnDisable() {
        buttonController.UnregisterOnPointerDown(ButtonDown);
        buttonController.UnregisterOnPointerUp(ButtonUp);
        buttonController.UnregisterOnPointerEnter(ButtonHovered);
        buttonController.UnregisterOnPointerExit(ButtonUnhovered);
    }

    private void OnValidate() {
        dialIntervalImage.fillAmount = fillAmount / angleMax;
        dialIntervalImage.transform.eulerAngles = new Vector3(0, 0, GetRotation(0f, intervalAngle, 0f));

        SetActive(toggleVisibility);
    }


    private void Start() {
        dialStartAngle += 90;
        dialEndAngle += 360 + 90;

        dialIntervalImage.fillAmount = fillAmount / angleMax;
        dialNeedle.transform.eulerAngles = new Vector3(0, 0, GetRotation(angle, dialEndAngle, dialStartAngle));

        SetActive(toggleVisibility);
        isActivated = false;
        OnStart();
    }

    protected virtual void OnStart() { }

    private void Update() {
        OnUpdate();

        if (lastToggledSide != toggleSide) {
            if (toggleSide == false) {
                side = Side.Left;
            }
            else {
                side = Side.Right;
            }
            SetSide(side);
        }
    }

    protected virtual void OnUpdate() { }

    private void ButtonDown(PointerEventData pointerEventData) {
        OnButtonDown();
    }
    protected virtual void OnButtonDown() { }

    private void ButtonUp(PointerEventData pointerEventData) {
        OnButtonUp();
    }

    protected virtual void OnButtonUp() { }

    private void ButtonHovered(PointerEventData pointerEventData) {
        dialButton.sprite = dialButtonHovered;
        OnButtonHovered();
    }

    protected virtual void OnButtonHovered() { }
    private void ButtonUnhovered(PointerEventData pointerEventData) {
        dialButton.sprite = dialButtonNormal;
        OnButtonUnhovered();
    }

    protected virtual void OnButtonUnhovered() { }

    protected float GetRotation(float angle, float endAngle, float startAngle) {
        float totalAngleSize = endAngle - startAngle;
        float angleNormalized = angle / angleMax;
        return endAngle - angleNormalized * totalAngleSize;
    }

    public void SetActive(bool toggle) {
        theDial.SetActive(toggle);
        thePanel.SetActive(toggle);

        indicator.sprite = indicatorRed;
        toggleVisibility = toggle;
        isActivated = toggle;
    }

    public void SetSide(Side side) {
        if (side == Side.Left) {
            theDial.transform.SetParent(leftSide.transform);
            thePanel.transform.SetParent(leftSide.transform);
            lastToggledSide = false;
            toggleSide = false;

        }
        else {
            theDial.transform.SetParent(rightSide.transform);
            thePanel.transform.SetParent(rightSide.transform);
            lastToggledSide = true;
            toggleSide = true;
        }
    }

}
